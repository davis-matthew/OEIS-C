/*
 * A364936  a(n) = minimum number of variables with n possible states in a
 *          system such that its solution requires the processing of a
 *          transcomputational number of bits.
 * https://oeis.org/A364936
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(93*log(10)/log(n))
 * Range:     u8 none, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: minimal k such that n^k >= 10^93   (for all n >= 2) */

#include <stdint.h>
#include <stddef.h>

/* Simple fixed‑size big integer (little endian limbs) */
typedef struct {
    uint64_t limb[6];      /* 6 limbs = 384 bits, enough for 10^93 (~320 bits) */
    size_t len;            /* number of used limbs (>=1) */
} BigInt;

/* Initialize BigInt with an unsigned integer */
static void big_from_uint(BigInt *x, uint64_t v) {
    x->len = (v == 0) ? 1 : 1;
    x->limb[0] = v;
    for (size_t i = 1; i < 6; ++i) x->limb[i] = 0;
}

/* Multiply BigInt by a small unsigned integer (<= 10000) */
static void big_mul_small(BigInt *x, uint64_t m) {
    unsigned __int128 prod;
    uint64_t carry = 0;
    for (size_t i = 0; i < x->len; ++i) {
        prod = (unsigned __int128)x->limb[i] * m + carry;
        x->limb[i] = (uint64_t)prod;
        carry = (uint64_t)(prod >> 64);
    }
    while (carry && x->len < 6) {
        x->limb[x->len++] = carry;
        carry = 0;
    }
}

/* Compare two BigInts: returns <0 if a<b, 0 if a==b, >0 if a>b */
static int big_cmp(const BigInt *a, const BigInt *b) {
    if (a->len != b->len)
        return (a->len < b->len) ? -1 : 1;
    for (size_t i = a->len; i-- > 0; ) {
        if (a->limb[i] != b->limb[i])
            return (a->limb[i] < b->limb[i]) ? -1 : 1;
    }
    return 0;
}

/* Global constant 10^93, lazily initialized */
static BigInt thr;
static int thr_initialized = 0;

static void init_thr(void) {
    if (thr_initialized) return;
    thr_initialized = 1;
    big_from_uint(&thr, 1);
    for (int i = 0; i < 93; ++i)
        big_mul_small(&thr, 10);
}

extern seq_t a(idx_t n) {
    if (n < 2) return (seq_t)0;   /* offset starts at 2 */

    init_thr();

    BigInt cur;
    big_from_uint(&cur, 1);
    unsigned long long k = 0;

    while (1) {
        ++k;
        big_mul_small(&cur, (uint64_t)n);
        if (big_cmp(&cur, &thr) >= 0)
            return (seq_t)k;
    }
}
