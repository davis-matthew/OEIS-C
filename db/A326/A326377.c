/*
 * A326377  For any number n > 0, let f(n) be the polynomial of a single
 *          indeterminate x where the coefficient of x^e is the
 *          prime(1+e)-adic valuation of n (where prime(k) denotes the k-th
 *          prime); f establishes a bijection between the positive numbers and
 *          the polynomials of a single indeterminate x with nonn...
 * https://oeis.org/A326377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = g(f(n) o f(n)) where f encodes prime exponents as
 *            polynomial coefficients
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 20, u64 n <= 21, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 32 (2026-11-03)
 * Helpers:   factor_u64, ipow, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For each n > 0, let f(n) be the polynomial ∑ e_k · x^k where e_k = v_{prime(k+1)}(n).
   Then a(n) is obtained by composing f(n) with itself and interpreting the coefficients
   again as exponents of successive primes. */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow.h"
#include "../utils/mem.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

#define MAX_DEG 20000          /* maximal degree needed for n ≤ 10000 */
#define MAX_IDX 2000           /* maximal prime index needed for n ≤ 10000 */

static seq_t base_coeff_by_index[MAX_IDX];
static seq_t cur_pow[MAX_DEG];
static seq_t tmp[MAX_DEG];
static seq_t result[MAX_DEG];

/* Computes the nth term of OEIS A326377 */
seq_t a(idx_t n)
{
    /* factor n */
    uint64_t primes[20], exps[20];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* reset coefficient lookup table */
    mem_set(base_coeff_by_index, 0, sizeof(base_coeff_by_index));

    /* fill coefficient table with prime exponents */
    size_t max_idx = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p   = primes[i];
        size_t idx = 0;
        while (get_prime((idx_t)(idx + 1)) != (uint32_t)p) ++idx; /* zero‑based index */
        base_coeff_by_index[idx] = (seq_t)exps[i];
        if (idx > max_idx) max_idx = idx;
    }
    size_t base_deg = (max_idx == 0 ? 0 : max_idx);

    /* clear working arrays */
    mem_set(cur_pow, 0, sizeof(cur_pow));
    mem_set(result, 0, sizeof(result));

    cur_pow[0] = (seq_t)1;   /* P^0 = 1 */
    size_t cur_deg = 1;      /* current degree of cur_pow */

    /* iterate over each coefficient index of the base polynomial */
    for (size_t e = 0; e <= base_deg; ++e) {
        seq_t coeff_e = base_coeff_by_index[e];

        /* add coeff_e * cur_pow to the result polynomial */
        if (coeff_e != 0) {
            for (size_t i = 0; i < cur_deg; ++i) {
                result[i] += coeff_e * cur_pow[i];
            }
        }

        if (e == base_deg) break;   /* no further powers needed */

        /* cur_pow = cur_pow * P (sparse multiplication) */
        size_t tmp_len = cur_deg + base_deg + 1;
        if (tmp_len > MAX_DEG) tmp_len = MAX_DEG;
        mem_set(tmp, 0, tmp_len * sizeof(seq_t));

        for (size_t idx = 0; idx <= base_deg; ++idx) {
            seq_t c = base_coeff_by_index[idx];
            if (c == 0) continue;
            for (size_t i = 0; i < cur_deg; ++i) {
                tmp[i + idx] += cur_pow[i] * c;
            }
        }

        /* trim tmp to find new degree */
        size_t new_deg = 1;
        for (size_t i = tmp_len; i-- > 0; ) {
            if (tmp[i]) { new_deg = i + 1; break; }
        }

        /* copy back to cur_pow */
        mem_copy(cur_pow, tmp, new_deg * sizeof(seq_t));
        cur_deg = new_deg;
    }

    /* trim result polynomial to find its actual degree */
    size_t res_last = MAX_DEG - 1;
    while (res_last > 0 && result[res_last] == 0) --res_last;
    size_t res_len = res_last + 1;

    /* convert the resulting polynomial back to an integer */
    seq_t ans = (seq_t)1;
    for (size_t k = 0; k < res_len; ++k) {
        seq_t coeff = result[k];
        if (coeff == 0) continue;
        seq_t prime = (seq_t)get_prime((idx_t)(k + 1));
        seq_t power = ipow(prime, coeff);
        ans = ans * power;
    }
    return ans;
}
