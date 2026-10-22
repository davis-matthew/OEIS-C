/*
 * A400390  Irregular table read by rows: T(n, k) = gcd(d(n, k), n/d(n, k)),
 *          where d(n, k) = A027750(n, k) is the k-th divisor of n.
 * https://oeis.org/A400390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) is the n‑th term of the sequence obtained by writing * the
 *            irregular table T(n,k)=gcd(d(n,k),n/d(n,k)) row by row. * The
 *            table row n contains τ(n) entries, where τ(n) is the number of *
 *            positive divisors of n. The sequence a(n) is the nth element *
 *            in this row‑by‑row listing (offset 1)
 * Range:     u8 n <= 9077, u16 n <= 9077, u32 n <= 9077, u64 n <= 9077,
 *            u128 n <= 9077, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"

static uint64_t divisor_count(uint64_t n)
{
    uint64_t cnt = 0;
    for (uint64_t i = 1; i <= n / i; ++i)
    {
        if (n % i == 0)
        {
            ++cnt;
            if (i != n / i)
                ++cnt;
        }
    }
    return cnt;
}

static int cmp_u64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

/* Return the kth (1‑based) divisor of n, in ascending order. */
static uint64_t kth_divisor(uint64_t n, uint64_t k)
{
    uint64_t divs[256];
    uint64_t cnt = 0;
    for (uint64_t i = 1; i <= n / i; ++i)
    {
        if (n % i == 0)
        {
            divs[cnt++] = i;
            if (i != n / i)
                divs[cnt++] = n / i;
        }
    }
    qsort(divs, cnt, sizeof(uint64_t), cmp_u64);
    return divs[k - 1];
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* Undefined below the offset; harmless. */

    idx_t row = 1;
    idx_t cumulative = 0;

    for (;; ++row)
    {
        uint64_t t = divisor_count((uint64_t)row);
        if (cumulative + t >= n)
            break;
        cumulative += t;
    }

    idx_t k = n - cumulative;   /* 1‑based within the found row */

    uint64_t d = kth_divisor((uint64_t)row, (uint64_t)k);
    uint64_t other = (uint64_t)row / d;
    return gcd((seq_t)d, (seq_t)other);
}
