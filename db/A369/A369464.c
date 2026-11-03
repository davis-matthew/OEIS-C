/*
 * A369464  Numbers for which there is no representation as a sum (p*q + p*r +
 *          q*r) with three odd primes p <= q <= r.
 * https://oeis.org/A369464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Non‑representable integers (offset 1). a(n) is the nth integer
 *            that cannot be written as p*q + p*r + q*r with three odd primes
 *            p ≤ q ≤ r
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Validated: partial, pass, 82 terms through n = 82 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64, isqrt, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"
#include "../utils/isqrt.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ----------  Prime table (dynamic, up to sqrt(max_checked)) --------------- */
static uint64_t *prime_list = NULL;   /* Stored primes (including 2)      */
static size_t   prime_cnt  = 0;
static idx_t    prime_limit = 0;      /* Largest value we have pre‑prime up */

static void
ensure_prime_limit(idx_t limit)
{
    if (limit <= prime_limit)
        return;

    uint64_t *new_list = NULL;
    size_t    new_cnt  = 0;
    build_primes_upto(limit, &new_list, &new_cnt);

    if (prime_list != NULL)
        free(prime_list);

    prime_list = new_list;
    prime_cnt  = new_cnt;
    prime_limit = limit;
}

/* ----------  Test if x is representable --------------------------------- */
static bool
is_representable(idx_t x)
{
    /* smallest representable value: 3*3+3*3+3*3 = 27 */
    if (x < 27)
        return false;

    /* we need primes up to sqrt(x) + 1 for the search */
    ensure_prime_limit(isqrt(x) + 1);

    idx_t p_max = isqrt(x / 3) + 1;      /* 3p^2 <= x  →  p <= sqrt(x/3) */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p == 2)                 /* skip even prime */
            continue;
        if (p > (uint64_t)p_max)
            break;

        for (size_t j = i; j < prime_cnt; ++j) {
            uint64_t q = prime_list[j];
            if (q == 2)
                continue;
            if (q < p)
                continue;

            /* if p*q already exceeds x, no further q will work */
            if ((uint64_t)p * (uint64_t)q > (uint64_t)x)
                break;

            /* r = (x - p*q) / (p + q) must be integer prime >= q   */
            uint64_t r_num = (uint64_t)x - (uint64_t)p * (uint64_t)q;
            if (r_num == 0)
                continue;  /* would give r = 0 */
            uint64_t denom = (uint64_t)p + (uint64_t)q;
            if (r_num % denom != 0)
                continue;
            uint64_t r = r_num / denom;
            if (r < q)
                continue;
            if ((r & 1ULL) == 0)          /* r must be odd */
                continue;
            /* final primality test */
            if (is_prime_big((seq_t)r))
                return true;
        }
    }
    return false;
}

/* ----------  Non‑representable cache ----------------------------------- */
static seq_t  *nonrep_list = NULL;  /* dynamic array */
static size_t  nonrep_cnt  = 0;
static size_t  nonrep_cap  = 0;
static idx_t   last_checked = 0;    /* next integer to test            */

static void
add_nonrep(idx_t val)
{
    if (nonrep_cnt >= nonrep_cap) {
        size_t new_cap = nonrep_cap ? 2 * nonrep_cap : 8;
        seq_t *tmp = mem_resize(seq_t, nonrep_list, new_cap);
        if (!tmp)
            abort(); /* out of memory */
        nonrep_list = tmp;
        nonrep_cap  = new_cap;
    }
    nonrep_list[nonrep_cnt++] = (seq_t)val;
}

/* ----------  Main sequence function ------------------------------------- */
seq_t
a(idx_t n)
{
    if (n == 0)           /* offset is 1; undefined behaviour for n=0 */
        return 0;

    /* generate non‑representable numbers until we have n of them */
    while (nonrep_cnt < (size_t)n) {
        if (!is_representable(last_checked))
            add_nonrep(last_checked);
        ++last_checked;
    }
    return nonrep_list[n - 1];
}
