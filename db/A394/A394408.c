/*
 * A394408  Lesser of two consecutive numbers of the form 2^i*5^j with i, j >=
 *          0, that are coprime.
 * https://oeis.org/A394408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lesser of two consecutive numbers of the form 2^i*5^j
 *            that are coprime. This implementation generates the set S = {
 *            2^i*5^j | i,j≥0 } in increasing order, and extracts the smaller
 *            element of each adjacent pair whose elements are coprime
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 9, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *S       = NULL;   /* sorted 2^i*5^j list */
    static size_t S_cap   = 0, S_len = 0;
    static idx_t i2 = 0, i5 = 0;     /* indices for next 2× and 5× multiples */

    static seq_t *A      = NULL;    /* sequence of coprime smaller elements */
    static size_t A_cap  = 0, A_len = 0;

    /* initialise the list S (first element is 1) */
    if (S == NULL) {
        S_cap = 64;
        S = (seq_t *)malloc(S_cap * sizeof(seq_t));
        if (!S) return (seq_t)0;   /* out–of–memory fallback */
        S_len = 1;
        S[0] = ((seq_t)1);
    }

    /* initialise the sequence A */
    if (A == NULL) {
        A_cap = 32;
        A = (seq_t *)malloc(A_cap * sizeof(seq_t));
        if (!A) return (seq_t)0;
    }

    /* generate until we have at least n terms in A */
    while (A_len < n) {
        /* ensure capacity in S */
        if (S_len == S_cap) {
            S_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(S, S_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;
            S = tmp;
        }

        /* next candidates */
        seq_t next2 = S[i2] * ((seq_t)2);
        seq_t next5 = S[i5] * ((seq_t)5);
        seq_t next;

        if (next2 < next5) {
            next = next2;
            ++i2;
        } else if (next5 < next2) {
            next = next5;
            ++i5;
        } else {
            next = next2;      /* equal: avoid duplication */
            ++i2;
            ++i5;
        }

        S[S_len++] = next;   /* append new element */

        /* process the last adjacent pair */
        if (S_len >= 2) {
            seq_t prev = S[S_len - 2];
            seq_t curr = S[S_len - 1];

            bool both_even = ((prev % ((seq_t)2)) == ((seq_t)0))
                           && ((curr % ((seq_t)2)) == ((seq_t)0));
            bool both_five = ((prev % ((seq_t)5)) == ((seq_t)0))
                           && ((curr % ((seq_t)5)) == ((seq_t)0));

            if (!(both_even || both_five)) {   /* gcd(prev,curr)==1 */
                if (A_len == A_cap) {
                    A_cap <<= 1;
                    seq_t *tmp = (seq_t *)realloc(A, A_cap * sizeof(seq_t));
                    if (!tmp) return (seq_t)0;
                    A = tmp;
                }
                A[A_len++] = prev; /* lesser element of the coprime pair */
            }
        }
    }

    return A[n - 1];
}
