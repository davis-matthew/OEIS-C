/*
 * A244924  Odd integers n such that for every integer k>0, n*2^k-1 has a
 *          divisor in the set { 3, 5, 7, 13, 17, 97, 673 }.
 * https://oeis.org/A244924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   mod_inv32, order_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A244924 – odd integers n such that for every integer k>0,
 *  n*2^k-1 has a divisor in {3,5,7,13,17,97,673}.
 *
 *  The sequence is periodic with period 2·P where
 *  P = 3·5·7·13·17·97·673 = 1514845605
 *  and 3029691210 = 2·P.
 *
 *  Computation:
 *  – For each prime p in the set we pre‑compute
 *      • the multiplicative order of 2 modulo p,
 *      • the residues of 2^e (e=0..order-1),
 *      • the modular inverse of 2^e modulo p,
 *      • the bit mask indicating which k (1≤k≤336) satisfy
 *        k≡e (mod order).
 *  – Enumerate all tuples of residues (one per prime).
 *    For each tuple that covers every k from 1 to 336
 *    (i.e. the bitwise OR of the masks equals the full mask)
 *    we build the corresponding odd n by the Chinese
 *    Remainder Theorem from the chosen inverses.
 *  – Store the 96 distinct base values < 2·P.
 *  – For any n≥1, a(n)=base[(n‑1) mod 96] + (n‑1)/96·3029691210.
 *
 *  The implementation is pure integer arithmetic,
 *  uses only C11 features, and works for any seq_t
 *  that can hold the computed values.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mod_inv32.h"
#include "../utils/order_of_two.h"

/* constants */
static const uint32_t primes[7] = {3,5,7,13,17,97,673};
#define NUM_PRIMES (sizeof(primes)/sizeof(primes[0]))
#define ORDER_LIMIT 336            /* lcm of the orders of 2 modulo the primes */
#define WORDS ((ORDER_LIMIT+63)/64)/* 6 */

#define P      1514845605ULL
#define PERIOD 3029691210ULL

/* pre‑computed tables */
static uint8_t      periods[NUM_PRIMES];
static uint32_t     pow2_mod[NUM_PRIMES][ORDER_LIMIT];
static uint32_t     inv2pow[NUM_PRIMES][ORDER_LIMIT];
static uint64_t    masks[NUM_PRIMES][ORDER_LIMIT][WORDS];

/* base solutions (96 values) */
static uint64_t base_vals[96];
static size_t   base_cnt = 0;
static bool     initialized = false;

/* full coverage bit set */
static uint64_t full_mask[WORDS];

/* --------------------------------------------------------------- */

/* --------------------------------------------------------------- */
/* recursive helper to generate the 96 base values */
static uint8_t  e_sel[NUM_PRIMES];
static uint64_t cur_mask[WORDS];

static void dfs(int depth)
{
    if (depth == NUM_PRIMES) {
        /* check full coverage */
        bool ok = true;
        for (int w = 0; w < WORDS; ++w)
            if (cur_mask[w] != full_mask[w]) { ok = false; break; }
        if (!ok) return;

        /* Chinese Remainder Theorem */
        uint64_t x = 0, m = 1;
        for (int i = 0; i < NUM_PRIMES; ++i) {
            uint32_t p = primes[i];
            uint32_t r = inv2pow[i][e_sel[i]];
            uint32_t xmod = (uint32_t)(x % p);
            uint32_t diff = (r + p - xmod) % p;
            uint32_t inv_m = mod_inv32((uint32_t)(m % p), p);
            uint32_t k = (uint64_t)diff * inv_m % p;
            x += m * k;
            m *= p;                /* m == P after final iteration */
        }

        /* ensure odd */
        if ((x & 1ULL) == 0ULL)
            x += P;

        /* store if new */
        if (base_cnt < 96) {
            bool dup = false;
            for (size_t i = 0; i < base_cnt; ++i)
                if (base_vals[i] == x) { dup = true; break; }
            if (!dup) base_vals[base_cnt++] = x;
        }
        return;
    }

    uint64_t saved[WORDS];
    memcpy(saved, cur_mask, sizeof(cur_mask));

    for (uint8_t e = 0; e < periods[depth]; ++e) {
        e_sel[depth] = e;
        for (int w = 0; w < WORDS; ++w)
            cur_mask[w] = saved[w] | masks[depth][e][w];
        dfs(depth + 1);
        memcpy(cur_mask, saved, sizeof(cur_mask));
    }
}

/* comparison function for qsort */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t*)a;
    uint64_t vb = *(const uint64_t*)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

/* --------------------------------------------------------------- */
/* initialises all tables and discovers the 96 base solutions */
static void init_sequence(void)
{
    /* periods, powers, inverses, masks */
    for (int i = 0; i < NUM_PRIMES; ++i) {
        uint32_t p = primes[i];
        periods[i] = order_of_two(p);

        uint32_t pow_val = 1; /* 2^0 */
        for (uint8_t e = 0; e < periods[i]; ++e) {
            pow2_mod[i][e] = pow_val;
            inv2pow[i][e] = mod_inv32(pow_val, p);

            /* build mask for this residue */
            uint64_t m[WORDS] = {0};
            for (int k = 1; k <= ORDER_LIMIT; ++k)
                if ((k % periods[i]) == e)
                    m[(k-1) >> 6] |= 1ULL << ((k-1) & 63u);
            memcpy(masks[i][e], m, sizeof(m));

            pow_val = (pow_val * 2) % p;
        }
    }

    /* full mask: all ORDER_LIMIT bits set */
    for (int w = 0; w < WORDS; ++w) full_mask[w] = 0ULL;
    for (int k = 0; k < ORDER_LIMIT; ++k)
        full_mask[k >> 6] |= 1ULL << (k & 63u);

    /* search for the 96 base values */
    uint64_t zero[WORDS] = {0};
    memcpy(cur_mask, zero, sizeof(cur_mask));
    dfs(0);

    /* sort values for deterministic order */
    qsort(base_vals, base_cnt, sizeof(uint64_t), cmp_uint64);
}

/* --------------------------------------------------------------- */
/* required sequence function */
seq_t a(idx_t n)
{
    if (!initialized) {
        init_sequence();
        initialized = true;
    }

    idx_t idx = n - 1;                  /* zero‑based index */
    uint64_t base = base_vals[idx % 96];
    uint64_t mult = (uint64_t)(idx / 96) * PERIOD;
    return (seq_t)(base + mult);
}
