/*
 * A354101  a(n) = phi(A267099(n)) - phi(n), where A267099 is fully
 *          multiplicative involution swapping the positions of 4k+1 and 4k+3
 *          primes, and phi is Euler totient function.
 * https://oeis.org/A354101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = phi(A267099(n)) - phi(n)
 * Range:     i8 n <= 41, i16 n <= 2186, i32 n <= 9048, i64 n <= 9048,
 *            i128 n <= 9048, big any n
 * Helpers:   build_primes_upto, calc_phi, factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"
#include "../utils/calc_phi.h"
#include "../utils/pow_uint.h"

/* --- Global tables for prime lists ------------------------------------- */
#define PRIME_LIMIT 30000          /* sufficient for n <= 10000 */

static bool initialized = false;
static uint64_t *all_primes = NULL;
static size_t all_cnt = 0;

static uint64_t *p1 = NULL;       /* primes ≡ 1 (mod 4), excluding 2 */
static size_t p1_cnt = 0;
static uint64_t *p3 = NULL;       /* primes ≡ 3 (mod 4) */
static size_t p3_cnt = 0;

/* ----------------------------------------------------------------------- */
static void init(void)
{
    if (initialized)
        return;
    /* Build prime list up to PRIME_LIMIT */
    build_primes_upto((idx_t)PRIME_LIMIT, &all_primes, &all_cnt);
    /* Allocate arrays for the two residue classes */
    p1 = (uint64_t *)malloc(all_cnt * sizeof(uint64_t));
    p3 = (uint64_t *)malloc(all_cnt * sizeof(uint64_t));
    p1_cnt = 0;
    p3_cnt = 0;
    for (size_t i = 0; i < all_cnt; ++i) {
        uint64_t p = all_primes[i];
        if (p == 2)
            continue;
        if (p % 4 == 1) {
            p1[p1_cnt++] = p;
        } else { /* p % 4 == 3 */
            p3[p3_cnt++] = p;
        }
    }
    initialized = true;
}

/* Binary search for uint64_t */
static bool binary_search_uint64(const uint64_t *arr, size_t len,
                                 uint64_t target, size_t *idx)
{
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        uint64_t v = arr[mid];
        if (v == target) {
            *idx = mid;
            return true;
        }
        if (v < target)
            lo = mid + 1;
        else
            hi = mid;
    }
    return false;
}

/* Map a single prime under the involution A267099 */
static uint64_t map_prime_u64(uint64_t p)
{
    if (p == 2)
        return 2;
    size_t idx;
    if (p % 4 == 1) {
        if (binary_search_uint64(p1, p1_cnt, p, &idx))
            return p3[idx];
    } else { /* p % 4 == 3 */
        if (binary_search_uint64(p3, p3_cnt, p, &idx))
            return p1[idx];
    }
    /* Fallback (should not occur for p <= PRIME_LIMIT) */
    return p;
}

/* Compute A267099(n) via prime factorisation */
static seq_t map_value(idx_t n)
{
    if (n <= 1)
        return (seq_t)n;            /* A267099(1) = 1 */
    static uint64_t fac[32];
    static uint64_t exp[32];
    size_t fac_cnt = 0;
    factor_u64((uint64_t)n, fac, exp, &fac_cnt);
    seq_t result = 1;
    for (size_t i = 0; i < fac_cnt; ++i) {
        uint64_t prime = fac[i];
        unsigned int e = (unsigned int)exp[i];
        uint64_t mapped = map_prime_u64(prime);
        seq_t pow_val = pow_uint(mapped, (idx_t)e);
        result = result * pow_val;
    }
    return result;
}

/* ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init();
    seq_t mapped_n = map_value(n);
    seq_t phi_m = calc_phi(mapped_n);
    seq_t phi_n = calc_phi((seq_t)n);
    return phi_m - phi_n;
}
