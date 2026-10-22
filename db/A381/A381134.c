/*
 * A381134  a(n) = 4*(-3)^n + 3*(-2)^n + 3*2^n - (-3)^n*2^(n + 1) + 2*3^n*(2^n
 *          - 2).
 * https://oeis.org/A381134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 4*(-3)^n + 3*(-2)^n + 3*2^n
 *                - (-3)^n*2^(n + 1) + 2*3^n*(2^n - 2)
 */

#include <stdlib.h>
#include <stdint.h>

/* external integer types (supplied by the build) */
extern seq_t   a(idx_t n);  /* declaration requested by the problem   */

/* Helper: grow a power‑of‑two cache up to at least n terms (1‑based) */
static seq_t *pow2_cache = NULL;
static idx_t  pow2_size  = 0;

static seq_t get_pow2(idx_t n)
{
    if (n == 0) return 1; /* unused but harmless */
    if (pow2_size < n) {
        size_t new_size = (size_t)n;
        pow2_cache = (seq_t *)realloc(pow2_cache, new_size * sizeof(seq_t));
        size_t i;
        if (pow2_size == 0) {           /* first term: 2^1 */
            pow2_cache[0] = (seq_t)2;
            i = 1;
        } else {
            i = (size_t)pow2_size;
        }
        for (; i < new_size; ++i)
            pow2_cache[i] = pow2_cache[i - 1] * (seq_t)2;
        pow2_size = new_size;
    }
    return pow2_cache[(size_t)n - 1];
}

/* Helper: grow a power‑of‑three cache up to at least n terms (1‑based) */
static seq_t *pow3_cache = NULL;
static idx_t  pow3_size  = 0;

static seq_t get_pow3(idx_t n)
{
    if (n == 0) return 1; /* unused but harmless */
    if (pow3_size < n) {
        size_t new_size = (size_t)n;
        pow3_cache = (seq_t *)realloc(pow3_cache, new_size * sizeof(seq_t));
        size_t i;
        if (pow3_size == 0) {           /* first term: 3^1 */
            pow3_cache[0] = (seq_t)3;
            i = 1;
        } else {
            i = (size_t)pow3_size;
        }
        for (; i < new_size; ++i)
            pow3_cache[i] = pow3_cache[i - 1] * (seq_t)3;
        pow3_size = new_size;
    }
    return pow3_cache[(size_t)n - 1];
}

seq_t a(idx_t n)
{
    /* compute required powers */
    seq_t two_pow = get_pow2(n);
    seq_t three_pow = get_pow3(n);

    /* parity of n : 0 = even, 1 = odd */
    int parity = (int)(n & 1ULL);

    /* accumulator for positive and negative contributions */
    seq_t pos = 0;
    seq_t neg = 0;

    /* Term 1: 4*(-3)^n */
    seq_t t1 = (seq_t)4 * three_pow;
    if (parity == 0)   pos += t1;   /* even: (+) */
    else               neg += t1;   /* odd : (-) */

    /* Term 2: 3*(-2)^n */
    seq_t t2 = (seq_t)3 * two_pow;
    if (parity == 0)   pos += t2;   /* even: (+) */
    else               neg += t2;   /* odd : (-) */

    /* Term 3: 3*2^n (always positive) */
    seq_t t3 = (seq_t)3 * two_pow;
    pos += t3;

    /* Term 4: - (-3)^n*2^(n+1) */
    seq_t tp2 = two_pow * (seq_t)2;          /* 2^(n+1) */
    seq_t t4 = three_pow * tp2;              /* |(-3)^n * 2^(n+1)| */
    if (parity == 0)   neg += t4;   /* even: product positive → negative */
    else               pos += t4;   /* odd : product negative → positive */

    /* Term 5: 2*3^n*(2^n - 2)  (always positive) */
    seq_t two_minus = two_pow - (seq_t)2;      /* 2^n - 2 */
    seq_t t5 = two_minus * three_pow;          /* (2^n-2)*3^n */
    t5 = t5 * (seq_t)2;                        /* *2 */
    pos += t5;

    /* Final value: pos - neg (pos ≥ neg for all n ≥ 1) */
    return pos - neg;
}
