/*
 * A245027  Divisors of 7^12 - 1.
 * https://oeis.org/A245027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the n-th divisor of 7^12-1 (13841287200) in ascending
 *            order
 * Range:     u8 n <= 1, u16 n <= 21, u32 n <= 838, u64 n <= 864,
 *            u128 n <= 864, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Total number of divisors of 13841287200. */
#define TOTAL_DIVISORS 864

/* Prime factorisation of 13841287200 = 2^5·3^2·5^2·13·19·43·181 */
struct factor { uint64_t p; unsigned e; };

/* Comparator for qsort. */
static int cmp_seq_t(const void *va, const void *vb)
{
    const seq_t *a = (const seq_t *)va;
    const seq_t *b = (const seq_t *)vb;
    if (*a < *b) return -1;
    if (*a > *b) return  1;
    return 0;
}

/* Recursive enumeration of all divisors. */
static void gen_divs(size_t idx, seq_t cur,
                     const struct factor *f, size_t nf,
                     seq_t *out, size_t *cnt)
{
    if (idx == nf) {
        out[(*cnt)++] = cur;
        return;
    }
    uint64_t p = f[idx].p;
    unsigned e = f[idx].e;
    seq_t val = cur;
    for (unsigned k = 0; k <= e; ++k) {
        gen_divs(idx + 1, val, f, nf, out, cnt);
        val = val * (seq_t)p;
    }
}

/* OEIS function. */
seq_t a(idx_t n)
{
    static const struct factor facs[7] = {
        { 2ULL, 5 },
        { 3ULL, 2 },
        { 5ULL, 2 },
        {13ULL, 1 },
        {19ULL, 1 },
        {43ULL, 1 },
        {181ULL, 1 }
    };
    static seq_t divisors[TOTAL_DIVISORS];
    static bool initialized = false;
    static size_t count = 0;

    if (!initialized) {
        gen_divs(0, (seq_t)1, facs, 7, divisors, &count);
        qsort(divisors, count, sizeof(seq_t), cmp_seq_t);
        initialized = true;
    }

    if (n < (idx_t)1 || n > (idx_t)TOTAL_DIVISORS) {
        return (seq_t)0;                /* out of bounds for this finite sequence */
    }
    return divisors[(size_t)(n - (idx_t)1)];
}
