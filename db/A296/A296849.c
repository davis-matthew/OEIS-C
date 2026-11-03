/*
 * A296849  Solution of the complementary equation a(n) = 2*a(n-1) + a(n-2) +
 *          b(n), where a(0) = 1, a(1) = 2, b(0) = 3, b(1) = 4, b(2) = 5, and
 *          (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A296849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 49, u128 n <= 99,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 952 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 2*a(n-1) + a(n-2) + b(n);
 * a(0) = 1, a(1) = 2
 * b(0) = 3, b(1) = 4, b(2) = 5
 * (a(n)) and (b(n)) are complementary increasing sequences.
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

/* binary search – does A contain val?  A[0..len-1] is sorted */
static bool contains_a(const seq_t *A, size_t len, seq_t val) {
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (A[mid] == val) return true;
        if (A[mid] < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return false;
}

seq_t a(idx_t n) {
    static seq_t *A = NULL;          /* a‑sequence */
    static size_t capA = 0, lenA = 0;
    static seq_t *B = NULL;          /* b‑sequence */
    static size_t capB = 0, lenB = 0;
    static seq_t next_candidate = 0; /* smallest unused natural number */

    /* ---- initialization ---- */
    if (lenA == 0) {
        /* a(0) = 1, a(1) = 2 */
        capA = 4;
        A = mem_new(seq_t, capA);
        A[0] = (seq_t)1;
        A[1] = (seq_t)2;
        lenA = 2;

        /* b(0) = 3, b(1) = 4, b(2) = 5 */
        capB = 8;
        B = mem_new(seq_t, capB);
        B[0] = (seq_t)3;
        B[1] = (seq_t)4;
        B[2] = (seq_t)5;
        lenB = 3;

        next_candidate = (seq_t)6; /* next unused natural number */
    }

    /* ---- extend sequences up to n ---- */
    if ((size_t)n >= lenA) {
        for (size_t i = lenA; i <= (size_t)n; ++i) {
            /* ----- determine b(i) -----
             * If i < lenB, we already have it.
             * Otherwise find the smallest unused natural number. */
            if (i >= lenB) {
                while (true) {
                    if (!contains_a(A, lenA, next_candidate)) {
                        /* grow B if necessary */
                        if (i + 1 > capB) {
                            size_t newCap = capB ? capB * 2 : 4;
                            if (newCap < i + 1) newCap = i + 1;
                            grow_seq_array(&B, &capB, newCap);
                        }
                        B[i] = next_candidate;
                        ++lenB;
                        ++next_candidate; /* advance to next candidate */
                        break;
                    }
                    ++next_candidate; /* skip used number */
                }
            }

            /* ----- compute a(i) = 2*a(i-1) + a(i-2) + b(i) -----
             * For i==lenA (==2) we have A[0],A[1] and B[2] ready. */
            seq_t val = A[i - 1];
            val = val + A[i - 1];
            val = val + A[i - 2];
            val = val + B[i];

            /* grow A if necessary */
            if (i + 1 > capA) {
                size_t newCap = capA ? capA * 2 : 4;
                if (newCap < i + 1) newCap = i + 1;
                grow_seq_array(&A, &capA, newCap);
            }
            A[i] = val;
            ++lenA;
        }
    }

    return A[(size_t)n];
}
