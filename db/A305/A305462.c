/*
 * A305462  Fixed points of A305458.
 * https://oeis.org/A305462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer (n ≥ 1) whose primorial‑base
 *            representation has all digits except possibly the most
 *            significant one equal to 0 or 1
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   trial_is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ----------
   Standard headers
   ---------- */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/trial_is_prime.h"

/* ----------
   Static state – sequence storage
   ---------- */
static size_t seq_len  = 0;        /* number of terms stored        */
static size_t seq_cap  = 0;        /* capacity of seq_arr            */
static seq_t *seq_arr   = NULL;    /* array of sequence values       */

/* ----------
   Static state – primes
   ---------- */
static size_t primes_cnt = 0;      /* number of primes stored       */
static size_t primes_cap = 0;      /* capacity of primes array      */
static uint64_t *primes   = NULL;  /* dynamic array of primes       */

/* ----------
   Static state – current scan value
   ---------- */
static seq_t cur_scan = 0;         /* next integer to test          */

/* ----------
   Ensure at least 'needed' primes are available
   ---------- */
static void ensure_primes(size_t needed)
{
    if (primes_cnt >= needed) return;

    if (primes_cap == 0) {
        primes_cap = 16;
        primes = (uint64_t*)malloc(primes_cap * sizeof(uint64_t));
    }
    while (primes_cnt < needed) {
        uint64_t candidate = (primes_cnt == 0) ? 2 : primes[primes_cnt - 1] + 1;
        while (!trial_is_prime(candidate)) ++candidate;

        if (primes_cnt == primes_cap) {
            primes_cap *= 2;
            primes = (uint64_t*)realloc(primes, primes_cap * sizeof(uint64_t));
        }
        primes[primes_cnt++] = candidate;
    }
}

/* ----------
   Check whether a given number satisfies the primorial‑base
   digit condition
   ---------- */
static bool satisfies(seq_t x)
{
    if (x == 0) return true;      /* 0 is a valid term */

    /* local buffer for digits – enough for the required range */
    unsigned char digits[128];
    size_t num_digits = 0;

    seq_t m = x;
    size_t i = 0;
    while (m > 0) {
        /* ensure we have the i‑th prime */
        ensure_primes(i + 1);

        /* digit extraction */
        seq_t prime = (seq_t)primes[i];      /* promote to seq_t */
        seq_t digit = m % prime;
        m          = m / prime;

        digits[num_digits++] = (unsigned char)(digit);

        ++i;
        if (num_digits >= sizeof(digits)/sizeof(digits[0]))
            return false;  /* safety: too many digits – not needed for n up to 10000 */
    }

    /* All digits except the most significant one must be 0 or 1 */
    for (size_t j = 0; j + 1 < num_digits; ++j)
        if (digits[j] > 1) return false;

    return true;
}

/* ----------
   Ensure that at least 'n' terms of the sequence are pre‑computed
   ---------- */
static void ensure_terms(idx_t n)
{
    if (seq_len >= (size_t)n) return;

    /* grow the storage array if necessary */
    if (seq_cap == 0) {
        seq_cap = 16;
        seq_arr = (seq_t*)malloc(seq_cap * sizeof(seq_t));
    }

    while (seq_len < (size_t)n) {
        /* find next admissible integer */
        while (true) {
            if (satisfies(cur_scan)) {
                /* store it */
                if (seq_len >= seq_cap) {
                    seq_cap *= 2;
                    seq_arr = (seq_t*)realloc(seq_arr, seq_cap * sizeof(seq_t));
                }
                seq_arr[seq_len++] = cur_scan;
                cur_scan = cur_scan + 1;   /* next integer to test */
                break;
            } else {
                cur_scan = cur_scan + 1;
            }
        }
    }
}

/* ----------
   Required sequence function
   ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* undefined, return 0 for safety */

    ensure_terms(n);
    return seq_arr[n - 1];
}
