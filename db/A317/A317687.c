/*
 * A317687  Lexicographically earliest sequence of distinct positive terms
 *          such that the sum of two consecutive terms is 3-smooth (A003586).
 * https://oeis.org/A317687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lexicographically earliest sequence of distinct positive integers
   such that a(n) + a(n+1) is 3‑smooth (A317687).  The sequence
   is built incrementally: for each new term we choose the
   smallest admissible positive integer that has not appeared
   before.  Uses simple dynamic arrays for the sequence,
   a set of used values, and the list of 3‑smooth numbers
   (Hamming numbers). */
#include <stdlib.h>
#include <stdbool.h>

/* internal dynamic array for the sequence */
static seq_t *seq_arr = NULL;
static uint64_t seq_len = 0;
static uint64_t seq_cap = 0;

/* array marking whether a value has been used */
static bool *used_arr = NULL;
static uint64_t used_cap = 0;

/* array of 3‑smooth numbers (Hamming numbers) */
static seq_t *hamming = NULL;
static uint64_t ham_len = 0;
static uint64_t ham_cap = 0;
static uint64_t i2 = 0, i3 = 0;   /* pointers for generating Hamming numbers */

/* ensure seq_arr has room for at least 'needed' elements */
static void grow_seq_arr(uint64_t needed)
{
    if (needed <= seq_cap) return;
    uint64_t new_cap = seq_cap ? seq_cap << 1u : 16;
    while (new_cap < needed) new_cap <<= 1u;
    seq_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* ensure used_arr has room for indices up to 'needed' - 1 */
static void grow_used_arr(uint64_t needed)
{
    if (needed <= used_cap) return;
    uint64_t new_cap = used_cap ? used_cap << 1u : 16;
    while (new_cap < needed) new_cap <<= 1u;
    bool *new_used = (bool *)realloc(used_arr, new_cap * sizeof(bool));
    for (uint64_t i = used_cap; i < new_cap; ++i) new_used[i] = false;
    used_arr = new_used;
    used_cap = new_cap;
}

/* ensure hamming array has at least 'len' elements (0‑based) */
static void grow_hamming(uint64_t len)
{
    if (ham_len > len) return;
    if (ham_len == 0) {
        ham_cap = 16;
        hamming = (seq_t *)calloc(ham_cap, sizeof(seq_t));
        hamming[0] = 1;                /* 1 is 3‑smooth */
        ham_len = 1;
        i2 = i3 = 0;
    }
    while (ham_len <= len) {
        seq_t next2 = hamming[i2] * (seq_t)2;
        seq_t next3 = hamming[i3] * (seq_t)3;
        seq_t next = next2 < next3 ? next2 : next3;
        if (next2 == next3) { ++i2; ++i3; }
        else if (next2 < next3) ++i2;
        else ++i3;

        if (ham_len == ham_cap) {
            ham_cap <<= 1u;
            hamming = (seq_t *)realloc(hamming, ham_cap * sizeof(seq_t));
        }
        hamming[ham_len++] = next;
    }
}

/* main function requested by the build */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; behaviour for n<1 is irrelevant */

    /* Initialise on first call */
    if (seq_len == 0) {
        grow_seq_arr(1);
        seq_arr[0] = 1;
        seq_len = 1;

        grow_used_arr(2);      /* we will use index 1 at least */
        used_arr[1] = true;

        grow_hamming(1);       /* initialise Hamming numbers */
    }

    /* Extend the sequence until it has at least n terms */
    while (seq_len < n) {
        seq_t prev = seq_arr[seq_len - 1];

        /* find the smallest admissible candidate */
        uint64_t j = 0;
        grow_hamming(ham_len);   /* ensure we have at least one element */

        /* locate first Hamming number > prev */
        while (j < ham_len && hamming[j] <= prev) ++j;

        for (;;) {
            if (j >= ham_len) {
                grow_hamming(ham_len);   /* generate more if needed */
                continue;
            }

            seq_t sum = hamming[j];
            seq_t cand = sum - prev;     /* candidate value */

            if (cand > 0) {
                size_t cand_idx = (size_t)cand;
                grow_used_arr(cand_idx + 1);
                if (!used_arr[cand_idx]) {
                    grow_seq_arr(seq_len + 1);
                    seq_arr[seq_len++] = cand;
                    used_arr[cand_idx] = true;
                    break;
                }
            }
            ++j;
        }
    }

    return seq_arr[n - 1];
}
