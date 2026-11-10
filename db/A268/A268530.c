/*
 * A268530  Positions of records in A268642.
 * https://oeis.org/A268530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index of the n-th record (left-to-right maximum) in
 *            Seelmann's sequence A268642: b(1)=1, b(k+1)=ceiling(b(k)/2)
 *            unless already present, else 3*b(k); computed by direct
 *            simulation
 * Range:     u8 n <= 8, u16 n <= 31, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A268530_seq = NULL;
static size_t A268530_cap = 0;
static size_t A268530_len = 0;
static seq_t *A268530_rec = NULL;
static size_t A268530_rcap = 0;
static size_t A268530_rlen = 0;
static seq_t A268530_max;

static bool A268530_seen(seq_t v)
{
    for (size_t i = 0; i < A268530_len; ++i)
        if (A268530_seq[i] == v)
            return true;
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    if (!A268530_seq) {
        A268530_cap = 64;
        A268530_seq = mem_new(seq_t, A268530_cap);
        A268530_rcap = 64;
        A268530_rec = mem_new(seq_t, A268530_rcap);
        A268530_seq[0] = (seq_t)1;
        A268530_len = 1;
        A268530_max = (seq_t)1;
        A268530_rec[0] = (seq_t)1;
        A268530_rlen = 1;
    }

    while (A268530_rlen < (size_t)n) {
        seq_t prev = A268530_seq[A268530_len - 1];
        seq_t cand = (prev >> 1) + (prev & (seq_t)1);
        seq_t next = A268530_seen(cand) ? (prev * (seq_t)3) : cand;

        if (A268530_len == A268530_cap) {
            size_t nc = A268530_cap * 2;
            seq_t *t = mem_new(seq_t, nc);
            for (size_t i = 0; i < A268530_len; ++i)
                t[i] = A268530_seq[i];
            mem_free(A268530_seq);
            A268530_seq = t;
            A268530_cap = nc;
        }
        A268530_seq[A268530_len] = next;
        ++A268530_len;

        if (next > A268530_max) {
            A268530_max = next;
            if (A268530_rlen == A268530_rcap) {
                size_t nc = A268530_rcap * 2;
                seq_t *t = mem_new(seq_t, nc);
                for (size_t i = 0; i < A268530_rlen; ++i)
                    t[i] = A268530_rec[i];
                mem_free(A268530_rec);
                A268530_rec = t;
                A268530_rcap = nc;
            }
            A268530_rec[A268530_rlen] = (seq_t)A268530_len;
            ++A268530_rlen;
        }
    }

    return A268530_rec[(size_t)n - 1];
}
