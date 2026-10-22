/*
 * A333906  For n >= 2, a(n) = Sum_{k=2..n} prevpower2(k) + nextpower2(k) -
 *          2*k, where prevpower2(k) is the largest power of 2 < k,
 *          nextpower2(k) is the smallest power of 2 > k.
 * https://oeis.org/A333906
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A333906
 *  a(n) = Sum_{k=2..n} (prevpower2(k) + nextpower2(k) - 2*k)
 *  where prevpower2(k) is the largest power of 2 < k
 *  and nextpower2(k) is the smallest power of 2 > k.  */
/*  The function works for any idx_t (uint64_t) and any seq_t
 *  (unsigned integer or big integer).  It makes no assumptions about
 *  call ordering and has a linear time complexity in n.  */

#include <stdint.h>

/* Compute floor(log2(u)) for a uint64_t value.  */
static inline unsigned
floor_log2_uint64(uint64_t u)
{
    unsigned r = 0;
    while (u >>= 1)
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    unsigned long long sum = 0ULL;

    for (idx_t k = 2; k <= n; ++k)
    {
        idx_t logk = floor_log2_uint64((uint64_t)k);
        unsigned long long prev;
        unsigned long long next;

        /* Determine prev and next powers of two.  */
        if ((k & (k - 1)) == 0)      /* k is a power of two */
        {
            prev = (unsigned long long)(k >> 1);
            /* next power after a power of two k is k<<1;
               use prev << 2 to avoid shifting k itself by 1,
               which would overflow for very large powers.  */
            next = prev << 2;
        }
        else
        {
            prev = 1ULL << logk;
            next = prev << 1;
        }

        unsigned long long bk = prev + next - 2ULL * k;
        sum += bk;
    }
    return (seq_t)sum;
}
