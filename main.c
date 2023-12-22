#include <stdio.h>
#include <unistd.h>
#include "lib/aflnet/alloc-inl.h"
#include "lib/aflnet/aflnet.h"

unsigned int *(*extract_response_codes)(unsigned char *buf, unsigned int buf_size, unsigned int *state_count_ref) = NULL;

int main(int argc, char *argv[]) {
    unsigned char *response_buf = NULL;
    unsigned int state_count;
    unsigned int *state_sequence;


    if (argc != 4) {
        PFATAL("Usage: %s protocol infile outfile", argv[0]);
    }

    if (!strcmp(argv[1], "RTSP")) extract_response_codes = &extract_response_codes_rtsp;
    else if (!strcmp(argv[1], "FTP")) extract_response_codes = &extract_response_codes_ftp;
    else if (!strcmp(argv[1], "DNS")) extract_response_codes = &extract_response_codes_dns;
    else if (!strcmp(argv[1], "DTLS12")) extract_response_codes = &extract_response_codes_dtls12;
    else if (!strcmp(argv[1], "DICOM")) extract_response_codes = &extract_response_codes_dicom;
    else if (!strcmp(argv[1], "SMTP")) extract_response_codes = &extract_response_codes_smtp;
    else if (!strcmp(argv[1], "SSH")) extract_response_codes = &extract_response_codes_ssh;
    else if (!strcmp(argv[1], "TLS")) extract_response_codes = &extract_response_codes_tls;
    else if (!strcmp(argv[1], "SIP")) extract_response_codes = &extract_response_codes_sip;
    else if (!strcmp(argv[1], "HTTP")) extract_response_codes = &extract_response_codes_http;
    else if (!strcmp(argv[1], "IPP")) extract_response_codes = &extract_response_codes_ipp;
    else if (!strcmp(argv[1], "SNMP")) extract_response_codes = &extract_response_codes_SNMP;
    else if (!strcmp(argv[1], "TFTP")) extract_response_codes = &extract_response_codes_tftp;
    else if (!strcmp(argv[1], "NTP")) extract_response_codes = &extract_response_codes_NTP;
    else if (!strcmp(argv[1], "DHCP")) extract_response_codes = &extract_response_codes_dhcp;
    else if (!strcmp(argv[1], "SNTP")) extract_response_codes = &extract_response_codes_SNTP;
    else {
        fprintf(stderr, "[AFLNet-parser] Protocol %s has not been supported yet!\n", argv[1]);
        exit(1);
    }

    FILE *input_file = fopen(argv[2], "rb");
    if (input_file == NULL) {
        fprintf(stderr, "Unable to open input file\n");
        exit(1);
    }

    fseek(input_file, 0L, SEEK_END);
    long response_buf_size = ftell(input_file);
    rewind(input_file);

    response_buf = ck_alloc(response_buf_size);
    if (response_buf == NULL) {
        fprintf(stderr, "Unable to allocate buffer\n");
        exit(1);
    }

    size_t bytes_read = fread(response_buf, 1, response_buf_size, input_file);
    if (bytes_read != response_buf_size) {
        fprintf(stderr, "Unable to read content of input file\n");
        exit(1);
    }
    fclose(input_file);

    FILE *output_file = fopen(argv[3], "wb+");
    if (output_file == NULL) {
        fprintf(stderr, "Unable to open output file\n");
        exit(1);
    }

    //Extract response codes
    state_sequence = (*extract_response_codes)(response_buf, response_buf_size, &state_count);

    printf("Responses from server:\n");
    for (int i = 0; i < state_count; i++) {
        printf("%d ", state_sequence[i]);
        fprintf(output_file, "%d ", state_sequence[i]);
    }
    printf("\n");
    fclose(output_file);

    ck_free(state_sequence);
    ck_free(response_buf);

    return 0;
}

