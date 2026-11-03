/*
 * A339853  Read one digit d at the time, starting from the first one; extend
 *          S with the smallest multiple of d not yet present in the sequence.
 *          Zeros are not read.
 * https://oeis.org/A339853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 203, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 96 terms through n = 4912 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A339853: S starts with 1; its digits are read one at a time from the first,
 * zeros skipped, and each digit d extends S with the smallest multiple of d not
 * yet in S. Generated in order with exact 64-bit values, a bitmap of the
 * values present and a cursor into the digits read so far. */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *S = NULL;
static size_t ns = 0, cap = 0;
static unsigned char *present = NULL;
static uint64_t pcap = 0;
static size_t rd_term = 0;                /* the term being read */
static int rd_pos = 0;                    /* digits of it already read */
static uint64_t next_mult[10];            /* smallest multiple of d possibly free */

static int has(uint64_t v) { return v < pcap && present[v]; }

static void add(uint64_t v)
{
    if (v >= pcap) {
        uint64_t c = pcap ? pcap : 4096;
        while (c <= v) c *= 2;
        unsigned char *t = (unsigned char *)realloc(present, (size_t)c);
        if (!t) abort();
        for (uint64_t i = pcap; i < c; ++i) t[i] = 0;
        present = t;
        pcap = c;
    }
    present[v] = 1;
    if (ns == cap) {
        cap = cap ? 2 * cap : 1024;
        uint64_t *t = (uint64_t *)realloc(S, cap * sizeof *t);
        if (!t) abort();
        S = t;
    }
    S[ns++] = v;
}

static int digit_at(uint64_t v, int pos)  /* pos-th digit from the left, -1 past the end */
{
    char buf[24];
    int len = 0;
    do { buf[len++] = (char)(v % 10); v /= 10; } while (v);
    return pos < len ? buf[len - 1 - pos] : -1;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (!ns) {
        for (int d = 1; d < 10; ++d) next_mult[d] = d;
        add(1);
    }
    while (ns < (size_t)n) {
        int d = digit_at(S[rd_term], rd_pos);
        if (d < 0) { ++rd_term; rd_pos = 0; continue; }
        ++rd_pos;
        if (d == 0) continue;
        uint64_t m = next_mult[d];
        while (has(m)) m += (uint64_t)d;
        next_mult[d] = m;
        add(m);
    }
    return (seq_t)S[n - 1];
}
