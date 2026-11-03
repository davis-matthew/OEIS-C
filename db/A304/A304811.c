/*
 * A304811  Solution (a(n)) of the complementary equation a(n) = b(2n) + b(5n)
 *          ; see Comments.
 * https://oeis.org/A304811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = b(2n) + b(5n); b(n) = least new (smallest unused positive
 *            integer)
 * Range:     u8 n <= 31, u16 n <= 8191, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 8957 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A304811: the complementary sequences a, b with b(n) the least positive
 * integer not yet placed and a(n) = b(2n) + b(5n), b(0) = 1, a(0) = 2 (the
 * program by Peter J. C. Moses in the entry): for each n >= 1 the five values
 * b(5n-4..5n) are placed, then a(n). Exact 64-bit values, a bitmap of the
 * numbers placed, and the terms cached. */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *A = NULL, *B = NULL;
static size_t na = 0, nb = 0, capa = 0, capb = 0;
static unsigned char *placed = NULL;
static uint64_t pcap = 0;

static void place(uint64_t v)
{
    if (v >= pcap) {
        uint64_t c = pcap ? pcap : 4096;
        while (c <= v) c *= 2;
        unsigned char *t = (unsigned char *)realloc(placed, (size_t)c);
        if (!t) abort();
        for (uint64_t i = pcap; i < c; ++i) t[i] = 0;
        placed = t;
        pcap = c;
    }
    placed[v] = 1;
}

static int is_placed(uint64_t v) { return v < pcap && placed[v]; }

static void push(uint64_t **arr, size_t *len, size_t *cap, uint64_t v)
{
    if (*len == *cap) {
        *cap = *cap ? 2 * *cap : 1024;
        uint64_t *t = (uint64_t *)realloc(*arr, *cap * sizeof *t);
        if (!t) abort();
        *arr = t;
    }
    (*arr)[(*len)++] = v;
}

seq_t a(idx_t n)
{
    if (!na) {
        push(&B, &nb, &capb, 1); place(1);
        push(&A, &na, &capa, 2); place(2);
    }
    while (na <= (size_t)n) {
        for (int i = 0; i < 5; ++i) {
            uint64_t v = B[nb - 1];
            while (is_placed(v)) ++v;           /* least new, from the last b */
            push(&B, &nb, &capb, v);
            place(v);
        }
        uint64_t v = B[nb - 1] + B[2 * na];     /* b(5n) + b(2n) */
        push(&A, &na, &capa, v);
        place(v);
    }
    return (seq_t)A[n];
}
