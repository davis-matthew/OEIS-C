/*
 * A356556  Parity of A061418.
 * https://oeis.org/A356556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A061418(n) mod 2
 * Range:     u8 n <= 108, u16 n <= 108, u32 n <= 108, u64 n <= 108,
 *            u128 n <= 108, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A061418 is defined by:  A(1) = 2,  A(n+1) = floor(3*A(n)/2). */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Size of the big‑integer buffer in 32‑bit limbs.
   Enough for n <= 10000 (≈ 6000 bits). */
#define LIMBS 256

/* Big integer represented as an array of 32‑bit limbs (least
   significant limb first). */
static uint32_t digits[LIMBS];
static size_t   digits_len = 1;   /* number of used limbs */

/* Parity cache:
   parity_cache[i] holds a(i) for i >= 1. */
static seq_t   *parity_cache = NULL;
static idx_t    cache_cap    = 0;   /* capacity of parity_cache (size)    */
static idx_t    cached_last  = 0;   /* last n for which parity_cache[n] is known */

/* Multiply the big integer by 3. */
static void mul3(void)
{
    uint64_t carry = 0;
    for (size_t i = 0; i < digits_len; ++i) {
        uint64_t prod = (uint64_t)digits[i] * 3 + carry;
        digits[i] = (uint32_t)prod;
        carry     = prod >> 32;
    }
    if (carry && digits_len < LIMBS) {
        digits[digits_len++] = (uint32_t)carry;
    }
}

/* Divide the big integer by 2 (floor). */
static void div2(void)
{
    uint32_t carry = 0;
    for (size_t i = digits_len; i-- > 0; ) {
        uint64_t cur = ((uint64_t)carry << 32) | digits[i];
        digits[i] = (uint32_t)(cur >> 1);
        carry     = (uint32_t)(cur & 1);
    }
    if (digits_len > 1 && digits[digits_len - 1] == 0) {
        --digits_len;
    }
}

/* Ensure the parity cache has room for indices up to n. */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)(n + 1);          /* indices 0..n inclusive */
    if (needed <= cache_cap) return;

    size_t newcap = needed;
    seq_t *tmp = (seq_t *)realloc(parity_cache, newcap * sizeof(seq_t));
    if (!tmp) return;                         /* out of memory – leave cache unchanged */

    if (!parity_cache) {
        for (size_t i = 0; i < newcap; ++i) tmp[i] = 0;
    } else {
        for (size_t i = cache_cap; i < newcap; ++i) tmp[i] = 0;
    }

    parity_cache = tmp;
    cache_cap    = newcap;
}

/* Compute a(k) for all k up to n. */
static void compute_upto(idx_t n)
{
    if (n == 0) return;

    /* Initialisation on first call. */
    if (cached_last == 0) {
        ensure_cache(n);
        digits[0]    = 2;          /* A(1) = 2 */
        digits_len   = 1;
        parity_cache[1] = (seq_t)(digits[0] & 1); /* a(1) = 0 */
        cached_last   = 1;
    }

    if (cached_last >= n) return;           /* already computed */

    ensure_cache(n);

    for (idx_t i = cached_last + 1; i <= n; ++i) {
        mul3();
        div2();                              /* now holds A(i) */
        parity_cache[i] = (seq_t)(digits[0] & 1); /* a(i) = parity of A(i) */
    }

    cached_last = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not defined for n < 1 */
    compute_upto(n);
    return parity_cache[n];
}
