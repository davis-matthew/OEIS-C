/*
 * A323782  Prime numbers such that the reverse of the balanced ternary
 *          representation is a prime or a negated prime.
 * https://oeis.org/A323782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   append_candidate, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 * Gen: primes p such that the reverse of the balanced ternary
 * representation of p is a prime or a negative prime.
 * The function is memoised and works for any order of calls.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime_big.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0; /* undefined for n < 1, safe fallback */
    }

    /* Memoisation state */
    static seq_t *terms = NULL;      /* array of found terms      */
    static size_t terms_len = 0;     /* number of stored terms    */
    static size_t terms_cap = 0;     /* capacity of the array     */
    static seq_t cand_start = (seq_t)2; /* next candidate to test */

    /* Generate terms until we have at least n of them */
    while (terms_len < (size_t)n) {
        /* Find the next prime candidate */
        seq_t cand = cand_start;
        while (!is_prime_big(cand)) {
            ++cand;
        }
        cand_start = cand + (seq_t)1;   /* next search starts here */

        /* Convert cand to balanced ternary digits (least‑significant first) */
        int8_t digits[256];   /* enough for even 128‑bit numbers */
        size_t dlen = 0;
        seq_t tmp = cand;
        while (tmp != (seq_t)0) {
            int rem = (int)(tmp % (seq_t)3);
            tmp = tmp / (seq_t)3;
            if (rem == 2) {
                rem = -1;
                ++tmp;
            }
            digits[dlen++] = (int8_t)rem;
        }
        if (dlen == 0) {
            digits[dlen++] = (int8_t)0;
        }

        /* Build the reversed balanced ternary number (most‑significant first) */
        __int128 rev_val = 0;   /* signed to handle negative values */
        for (size_t i = 0; i < dlen; ++i) {
            rev_val = rev_val * 3 + digits[i];
        }
        seq_t abs_rev = rev_val < 0 ? -(seq_t)rev_val : (seq_t)rev_val;

        /* Accept the prime if |reverse| is a prime > 1 */
        if (abs_rev > 1 && is_prime_big(abs_rev)) {
            append_candidate(&terms, &terms_len, &terms_cap, cand);
        }
    }

    return terms[n - 1];
}
