/*
 * A360587  a(n) is the least positive integer k such that k*(k+1)*...*(k+n-1)
 *          does not contain the digit 2, or -1 if there is no such k.
 * https://oeis.org/A360587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) = least positive integer k such that the product
 *            k·(k+1)·…·(k+n‑1) does not contain the digit 2, or -1 if no such
 *            k exists
 * Range:     u8 n <= 30, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define BIG_BASE 1000000000U            /* 10^9, base for internal big integer */
#define BIG_MAX   12000                 /* enough limbs for products up to ~10^108000 */

typedef struct {
    uint32_t d[BIG_MAX];               /* little‑endian limbs, base = BIG_BASE */
    size_t   l;                        /* number of used limbs */
} BigInt;

/* initialise BigInt to value v (0 <= v < BIG_BASE) */
static void big_init(BigInt *b, uint32_t v)
{
    b->l = 0;
    if (v == 0) return;
    b->d[0] = v;
    b->l    = 1;
}

/* multiply BigInt by small integer m (0 <= m < BIG_BASE) */
static void big_mul_small(BigInt *b, uint32_t m)
{
    if (m == 0) { b->l = 0; return; }
    uint64_t carry = 0;
    for (size_t i = 0; i < b->l; ++i) {
        uint64_t cur  = (uint64_t)b->d[i] * m + carry;
        b->d[i] = (uint32_t)(cur % BIG_BASE);
        carry   = cur / BIG_BASE;
    }
    while (carry) {
        if (b->l >= BIG_MAX) { b->l = BIG_MAX; return; }   /* safety guard */
        b->d[b->l++] = (uint32_t)(carry % BIG_BASE);
        carry /= BIG_BASE;
    }
}

/* test whether BigInt contains the decimal digit 2 */
static bool big_has_digit2(const BigInt *b)
{
    for (size_t i = 0; i < b->l; ++i) {
        uint32_t seg = b->d[i];
        /* each limb holds exactly 9 decimal digits (base 10^9) */
        for (int j = 0; j < 9; ++j) {
            if (seg % 10 == 2) return true;
            seg /= 10;
        }
    }
    return false;
}

/* main function required by the test harness */
seq_t a(idx_t n)
{
    /* Upper bound for k to avoid infinite loops */
    const idx_t K_LIMIT = 100000ULL;   /* ample for all cases in the known data */

    for (idx_t k = 1; k <= K_LIMIT; ++k) {
        BigInt prod;
        big_init(&prod, 1);            /* start with product = 1 */
        for (idx_t i = 0; i < n; ++i) {
            big_mul_small(&prod, (uint32_t)(k + i));
        }
        if (!big_has_digit2(&prod)) {
            return (seq_t)k;
        }
    }
    /* No satisfactory k found within the bound */
    return (seq_t)(-1);
}
