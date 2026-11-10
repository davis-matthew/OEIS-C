/*
 * A323476  Positions of 3's in A306211.
 * https://oeis.org/A323476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of the n-th 3 in A306211, where A306211 starts
 *            with [1] and repeatedly appends the run lengths of the current
 *            sequence
 * Range:     u8 n <= 15, u16 n <= 10517, u32 n <= 25000, u64 n <= 25000,
 *            u128 n <= 25000, big any n
 * Validated: standard, pass, 69 terms through n = 9731 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static unsigned int *s323476_seq = NULL;
static size_t s323476_len = 0, s323476_cap = 0;
static size_t *s323476_pos = NULL;
static size_t s323476_npos = 0, s323476_pcap = 0;
static size_t s323476_scanned = 0;

static void s323476_scan(void)
{
    for (; s323476_scanned < s323476_len; ++s323476_scanned) {
        if (s323476_seq[s323476_scanned] == 3) {
            if (s323476_npos == s323476_pcap) {
                s323476_pcap = s323476_pcap ? s323476_pcap * 2 : 64;
                s323476_pos = (size_t *)realloc(s323476_pos, s323476_pcap * sizeof(size_t));
            }
            s323476_pos[s323476_npos++] = s323476_scanned + 1;
        }
    }
}

static void s323476_grow(void)
{
    if (s323476_seq == NULL) {
        s323476_cap = 64;
        s323476_seq = (unsigned int *)malloc(s323476_cap * sizeof(unsigned int));
        s323476_seq[0] = 1;
        s323476_len = 1;
    }
    unsigned int *runs = (unsigned int *)malloc(s323476_len * sizeof(unsigned int));
    size_t cnt = 0, run = 1;
    for (size_t i = 0; i + 1 < s323476_len; ++i) {
        if (s323476_seq[i] == s323476_seq[i + 1]) ++run;
        else { runs[cnt++] = (unsigned int)run; run = 1; }
    }
    runs[cnt++] = (unsigned int)run;
    size_t need = s323476_len + cnt;
    if (need > s323476_cap) {
        while (s323476_cap < need) s323476_cap *= 2;
        s323476_seq = (unsigned int *)realloc(s323476_seq, s323476_cap * sizeof(unsigned int));
    }
    for (size_t j = 0; j < cnt; ++j) s323476_seq[s323476_len + j] = runs[j];
    s323476_len += cnt;
    free(runs);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (s323476_npos < (size_t)n) {
        s323476_grow();
        s323476_scan();
    }
    return (seq_t)s323476_pos[(size_t)n - 1];
}
