/*
 * A289815  The first of a pair of coprime numbers whose factorizations depend
 *          on the ternary representation of n (see Comments for precise
 *          definition).
 * https://oeis.org/A289815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 93, u16 n <= 1065, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A289815 – first of a coprime pair whose factorisation depends on the ternary representation of n.
   The algorithm follows the OEIS definition: start with the list of all prime powers >1.
   For each non‑zero ternary digit t∈{1,2} at exponent e (in increasing order) take the (e+1)‑st
   element of the current list, delete all higher powers of its prime from that point on.
   After all digits are processed the value of a(n) is the product of the
   (e+1)‑st elements of the final list corresponding to digits with t=1.  */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------ */
/* Prime power table – sufficient for n up to 10 000.           */
/* ------------------------------------------------------------ */
#define MAX_PRIME_POWERS 20000

static seq_t   pp_val[MAX_PRIME_POWERS];
static uint64_t pp_prime[MAX_PRIME_POWERS];
static size_t  pp_cnt  = 0;   /* number of prime powers stored      */
static uint64_t pp_next = 2;  /* next candidate to test            */

/* Simple primality test – fast enough for the small range.    */
static bool is_prime(uint64_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) return false;
    }
    return true;
}

/* Checks whether m is a prime power and returns its prime factor. */
static bool is_prime_power(uint64_t m, uint64_t *base) {
    if (m < 2) return false;
    uint64_t p = 0;
    if (m % 2 == 0) {
        p = 2;
    } else {
        for (uint64_t q = 3; q * q <= m; q += 2) {
            if (m % q == 0) { p = q; break; }
        }
    }
    if (p == 0) p = m;                     /* m itself is prime          */
    uint64_t t = m;
    while (t % p == 0) t /= p;
    if (t != 1) return false;
    *base = p;
    return true;
}

/* Generate prime powers until we have at least 'need' elements.   */
static void ensure_prime_powers(size_t need) {
    if (pp_cnt >= need) return;
    while (pp_cnt < need) {
        uint64_t m = pp_next++;
        uint64_t p;
        if (!is_prime_power(m, &p)) continue;
        pp_val[pp_cnt]   = (seq_t)m;
        pp_prime[pp_cnt] = p;
        ++pp_cnt;
    }
}

/* ------------------------------------------------------------ */
/* Main function required by the test harness.                  */
/* ------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    /* Extract ternary representation of n.                         */
    uint8_t exps[32];
    uint8_t digs[32];
    size_t k = 0;
    idx_t tmp = n;
    size_t pos = 0;
    while (tmp > 0) {
        uint8_t d = (uint8_t)(tmp % 3);
        if (d != 0) {
            exps[k] = (uint8_t)pos;
            digs[k] = d;
            ++k;
        }
        tmp /= 3;
        ++pos;
    }

    if (k == 0) return (seq_t)1;

    /* Find the largest exponent to decide how many prime powers to pre‑compute. */
    uint8_t max_e = 0;
    for (size_t i = 0; i < k; ++i) {
        if (exps[i] > max_e) max_e = exps[i];
    }

    /* Ensure we have a generous number of prime powers.                     */
    ensure_prime_powers((size_t)max_e + 2000);

    /* Mark removed elements – reused each call.                            */
    static bool removed[MAX_PRIME_POWERS];
    for (size_t i = 0; i < pp_cnt; ++i) removed[i] = false;

    /* Process each ternary digit in increasing order of exponent.          */
    for (size_t step = 0; step < k; ++step) {
        uint8_t exp = exps[step];

        /* Locate the exp‑th (zero‑based) element in the current list. */
        size_t idx_in_pp = 0, kept = 0;
        while (idx_in_pp < pp_cnt) {
            if (!removed[idx_in_pp]) {
                if (kept == exp) break;
                ++kept;
            }
            ++idx_in_pp;
        }
        if (idx_in_pp >= pp_cnt) return (seq_t)0; /* safety check */

        uint64_t prime_of_element = pp_prime[idx_in_pp];

        /* Delete all later powers of this prime.                 */
        for (size_t j = idx_in_pp + 1; j < pp_cnt; ++j) {
            if (!removed[j] && pp_prime[j] == prime_of_element) {
                removed[j] = true;
            }
        }
    }

    /* Compute the product of the required elements.                */
    seq_t result = (seq_t)1;
    for (size_t step = 0; step < k; ++step) {
        if (digs[step] != 1) continue;   /* digit 1 contributes */

        uint8_t exp = exps[step];

        /* Find the exp‑th element in the final list.         */
        size_t idx_in_pp = 0, kept = 0;
        while (idx_in_pp < pp_cnt) {
            if (!removed[idx_in_pp]) {
                if (kept == exp) break;
                ++kept;
            }
            ++idx_in_pp;
        }
        if (idx_in_pp >= pp_cnt) return (seq_t)0; /* safety */

        result = result * pp_val[idx_in_pp];
    }

    return result;
}
