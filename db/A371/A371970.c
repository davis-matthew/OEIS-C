/*
 * A371970  Exponents k such that the binary expansion of 3^k has an even
 *          number of ones.
 * https://oeis.org/A371970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth positive integer k such that popcount(3ᵏ) is even
 * Range:     u8 n <= 122, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   parity_uint32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/parity_uint32.h"

/*--------- Big integer helpers for multiplying by 3 -----------------*/
struct BigInt {
    uint32_t *limb;   /* little‑endian limbs, each 32 bits */
    size_t    len;    /* number of used limbs */
    size_t    cap;    /* allocated capacity          */
} big = {NULL, 0, 0};

static void big_init(void) {
    big.cap = 8;
    big.len = 0;
    big.limb = (uint32_t*)malloc(big.cap * sizeof(uint32_t));
}

static void big_grow(size_t need) {
    if (need > big.cap) {
        size_t newcap = big.cap ? big.cap * 2 : 8;
        while (newcap < need)
            newcap <<= 1;
        uint32_t *tmp = (uint32_t*)realloc(big.limb, newcap * sizeof(uint32_t));
        big.limb = tmp;
        big.cap = newcap;
    }
}

static void big_mul3(void) {
    uint64_t carry = 0;
    for (size_t i = 0; i < big.len; ++i) {
        uint64_t prod = (uint64_t)big.limb[i] * 3 + carry;
        big.limb[i] = (uint32_t)prod;
        carry = prod >> 32;
    }
    while (carry) {
        big_grow(big.len + 1);
        big.limb[big.len++] = (uint32_t)carry;
        carry >>= 32;
    }
}

/*--------- Parity of popcount of the whole big integer --------------*/
static int parity_big(void) {
    int p = 0;
    for (size_t i = 0; i < big.len; ++i)
        p ^= parity_uint32(big.limb[i]) & 1;
    return p;
}

/*--------- Cached sequence values -----------------------------------*/
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t len        = 0;      /* number of cached terms */
static idx_t  next_k     = 0;      /* next exponent to test    */

static void cache_ensure(size_t needed) {
    if (cache_cap < needed) {
        size_t newcap = cache_cap ? cache_cap * 2 : 8;
        while (newcap < needed)
            newcap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        cache = tmp;
        cache_cap = newcap;
    }
}

/*--------- Main function defined as required ---------------------*/
seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* undefined for offset 1 */

    if (!big.limb)   big_init();
    if (!cache)      cache_ensure(8);

    while (len < n) {
        /* find next exponent with even popcount */
        int found = 0;
        while (!found) {
            ++next_k;
            if (next_k == 1) {            /* initialise 3¹ = 3 */
                if (big.len == 0) {
                    big_grow(1);
                    big.limb[0] = 3;
                    big.len    = 1;
                }
            } else {
                big_mul3();              /* multiply by 3 */
            }
            if (parity_big() == 0)        /* even number of ones */
                found = 1;
        }
        cache_ensure(len + 1);
        cache[len++] = (seq_t)next_k;
    }
    return cache[n-1];
}
