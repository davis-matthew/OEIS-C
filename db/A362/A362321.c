/*
 * A362321  a(n) = n! * Sum_{k=0..floor(n/4)} n^k /(k! * (n-4*k)!).
 * https://oeis.org/A362321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/4)} n^k/(k!*(n-4*k)!)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t */

seq_t a(idx_t n)
{
    /* The term k = 0 contributes 1 to the sum. */
    seq_t sum = 1;

    if (n < 4)
        return sum;   /* a(0)=a(1)=a(2)=a(3)=1 */

    idx_t maxk = n / 4;
    if (maxk == 0)
        return sum;

#if defined(__SIZEOF_INT128__)   /* use 128‑bit intermediates for 64‑bit seq_t */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 T = 1;          /* current T_k as 128‑bit */
        for (idx_t k = 1; k <= maxk; ++k) {
            /* T_k = T_{k-1} * n * product_{i=1..4}(n-4k+i) / k */
            T *= (unsigned __int128)n;

            idx_t base = n - 4 * k;       /* n-4k, always >= 0 */
            T *= (unsigned __int128)(base + 4);
            T *= (unsigned __int128)(base + 3);
            T *= (unsigned __int128)(base + 2);
            T *= (unsigned __int128)(base + 1);

            T /= (unsigned __int128)k;

            sum += (seq_t)T;
        }
        return sum;
    }
    /* otherwise fall through to big‑integer path */
#endif

    /* For arbitrary‑precision seq_t (size > 8) we can compute directly
       without risking overflow because the class can grow as needed. */
    seq_t T = 1;
    for (idx_t k = 1; k <= maxk; ++k) {
        T *= (seq_t)n;

        idx_t base = n - 4 * k;       /* n-4k, >=0 */
        T *= (seq_t)(base + 4);
        T *= (seq_t)(base + 3);
        T *= (seq_t)(base + 2);
        T *= (seq_t)(base + 1);

        T /= (seq_t)k;

        sum += T;
    }
    return sum;
}
