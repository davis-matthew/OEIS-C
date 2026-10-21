/*
 * A280762  a(n) = ceiling( log_2( Catalan(n) ) ).
 * https://oeis.org/A280762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil( log_2( Catalan(n) ) )
 * Range:     u8 n <= 133, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation computes the exact Catalan number using a
    simple arbitrary‑precision integer (base 2^32).  Only a few
    elementary operations are required: multiplication by a small
    32‑bit integer, division by a small 32‑bit integer (exact), and
    bit length extraction.  The algorithm works for every n ≥ 0 and is
    efficient enough for n up to about 10 000. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --- simple big integer in base 2^32 ----------------------------------- */
typedef struct {
    size_t len;      /* number of words, len ≥ 1, little‑endian ordering   */
    uint32_t *w;     /* array of words */
} BigInt;

/* initialise with value 1 */
static void bi_init(BigInt *x)
{
    x->len = 1;
    x->w   = (uint32_t *)malloc(sizeof(uint32_t));
    *x->w  = 1;
}

/* free memory */
static void bi_free(BigInt *x)
{
    free(x->w);
    x->len = 0;
    x->w   = NULL;
}

/* multiply the big integer by a small 32‑bit integer m   */
static void bi_mul_small(BigInt *x, uint32_t m)
{
    uint64_t carry = 0;
    for (size_t i = 0; i < x->len; ++i) {
        uint64_t prod = (uint64_t)x->w[i] * m + carry;
        x->w[i] = (uint32_t)prod;
        carry   = prod >> 32;
    }
    while (carry) {
        x->w = (uint32_t *)realloc(x->w, (x->len + 1) * sizeof(uint32_t));
        x->w[x->len] = (uint32_t)carry;
        carry >>= 32;
        ++x->len;
    }
}

/* divide the big integer by a small 32‑bit integer d  (exact division) */
static void bi_div_small_exact(BigInt *x, uint32_t d)
{
    uint64_t rem = 0;
    for (int i = (int)x->len - 1; i >= 0; --i) {
        uint64_t cur = (rem << 32) | x->w[i];
        uint64_t q   = cur / d;
        rem = cur % d;
        x->w[i] = (uint32_t)q;
    }
    /* discard leading zero words */
    while (x->len > 1 && x->w[x->len - 1] == 0) {
        --x->len;
    }
    /* the division is exact, rem should be zero */
}

/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* compute Catalan(n) as a big integer */
    BigInt bi;
    bi_init(&bi);

    /* compute binom(2n,n) = Π_{k=1..n} (n+k)/k */
    for (size_t k = 1; k <= n; ++k) {
        bi_mul_small(&bi, (uint32_t)(n + k));   /* multiply by n+k  */
        bi_div_small_exact(&bi, (uint32_t)k);   /* exact divide by k */
    }
    /* finally divide by (n+1) to obtain Catalan(n) */
    bi_div_small_exact(&bi, (uint32_t)(n + 1));

    /* determine bit length and whether the value is a power of two */
    uint32_t msw = bi.w[bi.len - 1];
    int msb = 31 - __builtin_clz(msw);           /* most significant bit */
    int bitlen = (int)((bi.len - 1) * 32) + msb + 1; /* floor(log2)+1   */

    bool is_pow2 = true;
    /* check that all lower words are zero */
    for (size_t i = 0; i + 1 < bi.len; ++i) {
        if (bi.w[i] != 0) { is_pow2 = false; break; }
    }
    if (is_pow2) {
        /* check that the highest word has only one bit set */
        if ((msw & (msw - 1)) != 0) is_pow2 = false;
    }

    seq_t result = is_pow2 ? (seq_t)msb : (seq_t)bitlen;

    bi_free(&bi);
    return result;
}
