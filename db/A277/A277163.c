/*
 * A277163  Lexicographically least sequence of nonnegative integers that
 *          avoids 13/9-powers.
 * https://oeis.org/A277163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9935, u16 n <= 9935, u32 n <= 9935, u64 n <= 9935,
 *            u128 n <= 9935, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The lexicographically least sequence of nonnegative integers that avoids
   13/9‑powers.  The sequence is built greedily: a(n) is the smallest
   non‑negative integer that does not create a 13/9‑power in the prefix
   0..n.  A 13/9‑power of length 13k is a substring that can be written
   as a 9k‑prefix followed by a 4k‑suffix equal to the first 4k symbols
   of that prefix.  */


/* Standard utilities. */
#include <stdlib.h>
#include <stdbool.h>

/* Memoised storage for the sequence. */
static seq_t *seq   = NULL;  /* array of computed terms          */
static size_t seq_cap = 0;   /* current capacity of seq          */
static size_t seq_len = 0;   /* number of terms currently stored */

/* Grow the dynamic array to at least `need` elements. */
static void grow_seq(size_t need)
{
    if (need <= seq_cap)
        return;
    size_t newcap = seq_cap ? seq_cap << 1 : 1;
    while (newcap < need)
        newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
    seq = tmp;
    seq_cap = newcap;
}

/* Lexicographically least sequence of nonnegative integers that avoids
   13/9‑powers. */
seq_t a(idx_t n)
{
    /* Ensure we have computed up to index `n`. */
    if ((size_t)n + 1 > seq_len) {
        size_t need = (size_t)n + 1;
        grow_seq(need);

        /* Build terms from seq_len up to need-1. */
        for (size_t i = seq_len; i < need; ++i) {
            for (unsigned int cand = 0; ; ++cand) {
                bool ok = true;
                unsigned long long len = (unsigned long long)i + 1; /* current prefix length */

                /* Check each possible 13/9‑power ending at position i. */
                for (unsigned long long k = 1; 13ULL * k <= len; ++k) {
                    unsigned long long start  = len - 13ULL * k;
                    unsigned int   lenV = 9U * (unsigned int)k; /* length of 9k‑prefix   */
                    unsigned int   lenP = 4U * (unsigned int)k; /* length of 4k‑suffix   */
                    size_t posV = (size_t)start;
                    size_t posP = posV + lenV;

                    /* Test whether the 4k suffix equals the first 4k of the 9k‑prefix. */
                    bool equal = true;
                    for (unsigned int t = 0; t < lenP; ++t) {
                        seq_t v_val = seq[posV + t];
                        seq_t next_val = (posP + t == i) ? (seq_t)cand : seq[posP + t];
                        if (v_val != next_val) {
                            equal = false;
                            break;
                        }
                    }
                    if (equal) {          /* a 13/9‑power would be formed */
                        ok = false;
                        break;
                    }
                }

                if (ok) {                /* candidate does not create a forbidden block */
                    seq[i] = (seq_t)cand;
                    break;
                }
            }
        }
        seq_len = need;
    }
    return seq[(size_t)n];
}
