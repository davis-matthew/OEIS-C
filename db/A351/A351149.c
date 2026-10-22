/*
 * A351149  a(n) is the least exponent k such that the Hamming weight of
 *          n^(k+1) is not greater than the Hamming weight of n^k.
 * https://oeis.org/A351149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal k ≥ 1 such that popcount(n^(k+1)) ≤ popcount(n^k)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Simple small‑big integer (base 2^32) ------------------------------------ */
typedef struct {
    uint32_t *d;   /* least significant word first */
    size_t   len;  /* number of used words */
} BigInt;

/* initialise big integer with a small value */
static void bi_init(BigInt *x, uint64_t val)
{
    /* val fits in 64 bits; we store it as up to two 32‑bit words */
    size_t sz = (val >> 32) ? 2 : (val ? 1 : 1);
    x->d  = (uint32_t *)malloc(sz * sizeof(uint32_t));
    x->len = sz;
    x->d[0] = (uint32_t)(val & 0xffffffffu);
    x->d[1] = (uint32_t)(val >> 32);
}

/* multiply big integer by a 32‑bit non‑zero factor (in place) */
static void bi_mul_small(BigInt *x, uint32_t m)
{
    uint64_t carry = 0;
    for (size_t i = 0; i < x->len; ++i) {
        uint64_t prod = (uint64_t)x->d[i] * m + carry;
        x->d[i] = (uint32_t)prod;
        carry   = prod >> 32;
    }
    if (carry) {
        x->d = (uint32_t *)realloc(x->d, (x->len + 1) * sizeof(uint32_t));
        x->d[x->len] = (uint32_t)carry;
        x->len++;
    }
}

/* popcount of a big integer */
static int bi_popcount(const BigInt *x)
{
    int c = 0;
    for (size_t i = 0; i < x->len; ++i)
        c += __builtin_popcount(x->d[i]);
    return c;
}

/* free big integer */
static void bi_free(BigInt *x)
{
    free(x->d);
}

/* -------------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* safety for offset 0 */

    /* base fits in 32 bits for the input range used by the tests */
    uint32_t base = (uint32_t)n;

    BigInt cur;
    bi_init(&cur, base);                      /* cur = n^1 */
    int prev_pop = bi_popcount(&cur);

    for (uint64_t k = 1;; ++k) {             /* k ≥ 1 */
        bi_mul_small(&cur, base);             /* cur = n^(k+1) */
        int cur_pop = bi_popcount(&cur);
        if (cur_pop <= prev_pop) {
            bi_free(&cur);
            return (seq_t)k;                  /* a(n) found */
        }
        prev_pop = cur_pop;
    }
}
