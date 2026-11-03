/*
 * A348470  a(n) = lpf(EKG(n)) = A020639(A064413(n)).
 * https://oeis.org/A348470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 486, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 100 terms through n = 9845 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = lpf(EKG(n)) by generating the EKG sequence (OEIS A064413)
   and returning its least prime factor (OEIS A020639). */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return the smallest prime factor of n   (n > 1) */
static inline idx_t lpf(idx_t n)
{
    if (n % 2 == 0) return 2;
    for (idx_t i = 3; i <= n / i; i += 2)
        if (n % i == 0) return i;
    return n;   /* n itself is prime – should not be called for primes */
}

seq_t a(idx_t n)
{
    static seq_t *ekg = NULL;          /* EKG sequence terms   */
    static seq_t *lpf_arr = NULL;      /* lpf of each EKG term */
    static size_t ekg_cap = 0;         /* capacity of the above arrays   */
    static size_t ekg_len = 0;         /* number of terms stored         */
    static uint8_t *used = NULL;       /* bitmap of numbers already used */
    static size_t used_cap = 0;        /* size of bitmap                 */

    /* initialise */
    if (!ekg) {
        ekg_cap = 64;
        ekg = mem_new(seq_t, ekg_cap);
        lpf_arr = mem_new(seq_t, ekg_cap);

        ekg[0] = (seq_t)1;
        ekg[1] = (seq_t)2;
        lpf_arr[0] = (seq_t)1;          /* lpf(1) = 1 by definition */
        lpf_arr[1] = (seq_t)2;          /* lpf(2) = 2                 */

        ekg_len = 2;

        used_cap = 3;
        used = (uint8_t*)calloc(used_cap, 1);
        used[1] = 1;
        used[2] = 1;
    }

    /* generate terms up to requested index */
    while (ekg_len < (size_t)n) {
        seq_t prev = ekg[ekg_len - 1];
        size_t best = 0;                 /* best candidate for next term */

        /* enumerate distinct prime factors of prev */
        idx_t temp = (idx_t)prev;
        idx_t lastp = 0;
        while (temp > 1) {
            idx_t p = lpf(temp);         /* least prime factor */
            if (p != lastp) {
                size_t cand = (size_t)p;
                while (1) {
                    /* grow bitmap if necessary */
                    if (cand >= used_cap) {
                        size_t new_cap = used_cap * 2;
                        while (new_cap <= cand) new_cap *= 2;
                        uint8_t *tmp = (uint8_t*)realloc(used, new_cap);
                        memset(tmp + used_cap, 0, new_cap - used_cap);
                        used = tmp;
                        used_cap = new_cap;
                    }
                    if (!used[cand]) {
                        if (best == 0 || cand < best) best = cand;
                        break;
                    }
                    cand += (size_t)p;
                }
                lastp = p;
            }
            while (temp % p == 0) temp /= p;
        }

        ekg[ekg_len] = (seq_t)best;

        /* least prime factor of the new term */
        seq_t lpf_val;
        if (best == 1) {
            lpf_val = (seq_t)1;
        } else {
            lpf_val = (seq_t)lpf((idx_t)best);
        }
        lpf_arr[ekg_len] = lpf_val;

        /* mark new term as used */
        if (best >= used_cap) {
            size_t new_cap = used_cap * 2;
            while (new_cap <= (size_t)best) new_cap *= 2;
            uint8_t *tmp = (uint8_t*)realloc(used, new_cap);
            memset(tmp + used_cap, 0, new_cap - used_cap);
            used = tmp;
            used_cap = new_cap;
        }
        used[best] = 1;

        ekg_len++;

        /* grow the EKG and lpf arrays if needed */
        if (ekg_len >= ekg_cap) {
            ekg_cap *= 2;
            ekg = mem_resize(seq_t, ekg, ekg_cap);
            lpf_arr = mem_resize(seq_t, lpf_arr, ekg_cap);
        }
    }

    return lpf_arr[n - 1];
}
