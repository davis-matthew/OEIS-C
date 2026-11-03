/*
 * A241504  a(n) = |{0 < g < prime(n): g is not only a primitive root modulo
 *          prime(n) but also a partition number given by A000041}|.
 * https://oeis.org/A241504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integers g with 0 < g < prime(n) such that * g
 *            is a primitive root mod prime(n) and g is a partition number
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9123 (2026-11-01)
 * Helpers:   powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/powmod_u64.h"

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

/* Factor phi = p-1 into distinct prime factors; result in factors[], *cnt */
static void factor_phi(uint64_t phi, uint64_t *factors, size_t *cnt)
{
    *cnt = 0;
    if (phi % 2 == 0) {
        factors[(*cnt)++] = 2;
        while (phi % 2 == 0)
            phi /= 2;
    }
    for (uint64_t i = 3; i * i <= phi; i += 2) {
        if (phi % i == 0) {
            factors[(*cnt)++] = i;
            while (phi % i == 0)
                phi /= i;
        }
    }
    if (phi > 1)
        factors[(*cnt)++] = phi;
}

/* Pre‑computed partition numbers up to a value > 200000 (sufficient for primes <= 1e5) */
static seq_t part_vals[200];
static size_t part_cnt = 0;
static bool part_init = false;

static void init_partitions(void)
{
    if (part_init) return;
    part_init = true;

    const int MAXN = 200;        /* enough for values up to ~1e5 */
    int64_t ptmp[MAXN + 1];
    ptmp[0] = 1;
    for (int n = 1; n <= MAXN; ++n) {
        int64_t sum = 0;
        for (int k = 1; ; ++k) {
            int pent1 = k * (3 * k - 1) / 2;
            if (pent1 > n) break;
            int sign = (k & 1) ? 1 : -1;
            sum += sign * ptmp[n - pent1];
            int pent2 = k * (3 * k + 1) / 2;
            if (pent2 <= n)
                sum += sign * ptmp[n - pent2];
        }
        ptmp[n] = sum;
    }

    /* store until value exceeds 200000 (enough for all needed g) */
    for (int i = 0; i <= MAXN; ++i) {
        if ((seq_t)ptmp[i] > 200000ULL) {
            part_cnt = i;
            break;
        }
        part_vals[i] = (seq_t)ptmp[i];
    }
}

/* Check if a given unsigned integer is a partition number */
static bool is_partition(uint64_t g)
{
    init_partitions();
    for (size_t i = 0; i < part_cnt; ++i) {
        seq_t val = part_vals[i];
        if (val == (seq_t)g)
            return true;
        if (val > (seq_t)g)
            break;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1, but safe */
    uint64_t p = get_prime((idx_t)n);     /* prime(n), 1‑indexed */
    uint64_t phi = p - 1;
    uint64_t factors[64];                 /* enough for phi <= 1e5 */
    size_t fac_cnt;
    factor_phi(phi, factors, &fac_cnt);

    seq_t count = 0;
    for (uint64_t g = 1; g < p; ++g) {
        if (!is_partition(g))
            continue;

        bool ok = true;
        for (size_t i = 0; i < fac_cnt; ++i) {
            uint64_t q = factors[i];
            if (powmod_u64(g, phi / q, p) == 1) {
                ok = false;
                break;
            }
        }
        if (ok)
            ++count;
    }
    return count;
}
