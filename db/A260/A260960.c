/*
 * A260960  Least positive integer k < prime(n) such that there are 0 < i < j
 *          with i^2 + j^2 = k^2 for which (i*j)/2 is a primitive root modulo
 *          prime(n), or 0 if no such k exists.
 * https://oeis.org/A260960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k < prime(n) such that there exist 0 < i < j < k
 *            with i² + j² = k² for which (i*j)/2 is a primitive root modulo
 *            prime(n), or 0 if no such k exists
 * Range:     u8 n <= 1288, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd_int, get_prime, mul_mod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---- Standard headers ---- */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---- Utility includes ---- */
#include "../utils/get_prime.h"        /* get_prime(idx_t) */
#include "../utils/factor_u64.h"      /* factor_u64(uint64_t, uint64_t *, uint64_t *, size_t *) */
#include "../utils/gcd_int.h"         /* gcd_int(int,int) */
#include "../utils/mul_mod64.h"       /* mul_mod64(uint64_t,uint64_t,uint64_t) */

/* ---- Data structures for base Pythagorean triples ---- */
typedef struct {
    uint32_t i;          /* leg i */
    uint32_t j;          /* leg j */
    uint32_t k;          /* hypotenuse k (base hyp) */
    uint64_t area;       /* area of base triple: i*j/2 */
} Triple;

/* ---- Global cache of base triples ---- */
static Triple *base_triples = NULL;
static size_t base_count = 0;
static size_t base_capacity = 0;
static bool base_initialized = false;

/* ---- Helper to append a new triple ---- */
static void add_base_triple(uint32_t i, uint32_t j, uint32_t k, uint64_t area)
{
    if (base_count == base_capacity) {
        size_t new_cap = (base_capacity == 0) ? 16 : base_capacity * 2;
        Triple *new_ptr = (Triple *)realloc(base_triples, new_cap * sizeof(Triple));
        if (!new_ptr) return; /* allocation failure – implementation defined */
        base_triples = new_ptr;
        base_capacity = new_cap;
    }
    base_triples[base_count].i = i;
    base_triples[base_count].j = j;
    base_triples[base_count].k = k;
    base_triples[base_count].area = area;
    base_count++;
}

/* ---- Comparison for qsort (ascending k) ---- */
static int cmp_triple(const void *a, const void *b)
{
    const Triple *ta = (const Triple *)a;
    const Triple *tb = (const Triple *)b;
    if (ta->k < tb->k) return -1;
    if (ta->k > tb->k) return 1;
    return 0;
}

/* ---- Build all primitive base triples with hyp < limit (inclusive) ---- */
static void build_base_triples(uint64_t limit)
{
    if (base_initialized) return;
    uint32_t m = 2;
    for (;;) {
        uint64_t m2 = (uint64_t)m * m;
        if (m2 + 1 >= limit) break; /* smallest n=1 gives hyp=m²+1 */
        for (uint32_t n = 1; n < m; ++n) {
            if (((m - n) & 1) == 0) continue;          /* same parity → skip */
            if (gcd_int((int)m, (int)n) != 1) continue; /* not coprime → skip */

            uint64_t hyp = m2 + (uint64_t)n * n;
            if (hyp >= limit) break;                   /* further n increase hyp */
            uint64_t i_leg = m2 - (uint64_t)n * n;
            uint64_t j_leg = 2ULL * m * n;
            uint64_t area = i_leg * (j_leg / 2);        /* j_leg is even */

            add_base_triple((uint32_t)i_leg, (uint32_t)j_leg,
                            (uint32_t)hyp, area);
        }
        ++m;
    }

    /* Sort triples by ascending hyp for efficient search */
    qsort(base_triples, base_count, sizeof(Triple), cmp_triple);
    base_initialized = true;
}

/* ---- Primitive root test using 64‑bit modular exponentiation ---- */
static uint64_t powmod64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1 % mod;
    while (e) {
        if (e & 1) r = mul_mod64(r, a, mod);
        a = mul_mod64(a, a, mod);
        e >>= 1;
    }
    return r;
}

static bool is_primitive_root_uint64(uint64_t g, uint64_t p,
                                     const uint64_t *primes, size_t cnt)
{
    if (g <= 1) return false;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t q = primes[i];
        uint64_t exp = (p - 1) / q;
        if (powmod64(g, exp, p) == 1) return false;
    }
    return true;
}

/* ---- Main sequence function ---- */
seq_t a(idx_t n)
{
    /* Ensure base triples are available (limit comfortably > max expected prime). */
    if (!base_initialized) build_base_triples(200001); /* prime(10000)=104729 */

    /* 1. Prime of index n */
    uint64_t prime = (uint64_t)get_prime(n);

    /* 2. Factor p-1 to obtain distinct prime divisors */
    uint64_t pf[64], ex[64];
    size_t pf_cnt = 0;
    factor_u64(prime - 1, pf, ex, &pf_cnt);

    /* Keep only distinct primes */
    uint64_t distinct[64];
    size_t ndist = 0;
    for (size_t i = 0; i < pf_cnt; ++i) {
        bool seen = false;
        for (size_t j = 0; j < ndist; ++j)
            if (distinct[j] == pf[i]) { seen = true; break; }
        if (!seen) distinct[ndist++] = pf[i];
    }

    /* 3. Search for smallest k */
    uint64_t min_k = 0;   /* 0 denotes 'not found yet' */

    size_t idx = 0;
    while (idx < base_count && base_triples[idx].k < prime) {
        const Triple *bt = &base_triples[idx];
        uint64_t base_hyp = bt->k;
        uint64_t max_t = (prime - 1) / base_hyp;
        uint64_t base_area_mod = bt->area % prime;

        for (uint64_t t = 1; t <= max_t; ++t) {
            uint64_t k = t * base_hyp;
            if (k >= prime) break;
            if (min_k && k >= min_k) break;  /* further t will increase k */

            /* area_mod = (base_area mod p) * t^2 mod p */
            uint64_t t_mod = t % prime;
            __uint128_t prod = (__uint128_t)base_area_mod * t_mod % prime;
            prod = prod * t_mod % prime;
            uint64_t area_mod = (uint64_t)prod;

            if (area_mod <= 1) continue;   /* cannot be primitive root */

            if (is_primitive_root_uint64(area_mod, prime, distinct, ndist)) {
                if (min_k == 0 || k < min_k) min_k = k;
                /* we can break inner loop because next t gives larger k */
                break;
            }
        }
        ++idx;
    }

    return (seq_t)min_k;   /* 0 if none found */
}
