/*
 * A281814  Expansion of f(x, x^8) in powers of x where f(, ) is Ramanujan's
 *          general theta function.
 * https://oeis.org/A281814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n = 0 or n = 9k(k-1)/2 + k or n = 9k(k-1)/2 + 8k *
 *            for some integer k ≥ 1, otherwise a(n) = 0
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Calculate the integer square‑root of an unsigned 128–bit integer.
 *  Returns floor(sqrt(x)). */
static uint64_t isqrt_uint128(unsigned __int128 x)
{
    uint64_t low = 0, high = UINT64_MAX, mid, ans = 0;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        unsigned __int128 m2 = (unsigned __int128)mid * (unsigned __int128)mid;
        if (m2 <= x) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */

    /*  D = 49 + 72·n  (fits in 128 bits for all n < 2^64) */
    unsigned __int128 D = (unsigned __int128)72 * n + 49;

    uint64_t s = isqrt_uint128(D);
    if ((unsigned __int128)s * s != D) return (seq_t)0;  /* not a perfect square */

    /*  First form: (9k(k-1))/2 + k = n  ->  9k² – 7k – 2n = 0
     *  gives k = (7 + s) / 18   (s = sqrt(49 + 72n)) */
    if ((7 + s) % 18 == 0) {
        uint64_t k = (7 + s) / 18;
        if (k >= 1) return (seq_t)1;
    }

    /*  Second form: (9k(k-1))/2 + 8k = n  ->  9k² + 7k – 2n = 0
     *  gives k = (s – 7) / 18   (s = sqrt(49 + 72n)) */
    if (s >= 7 && (s - 7) % 18 == 0) {
        uint64_t k = (s - 7) / 18;
        if (k >= 1) return (seq_t)1;
    }

    return (seq_t)0;
}
