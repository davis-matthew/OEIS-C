/*
 * A279522  Number of ways to write n as w^2 + x^2 + y^2 + z^2 with w + 2*x +
 *          3*y + 5*z a square, where w,x,y,z are nonnegative integers.
 * https://oeis.org/A279522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The implementation computes a(n) = number of non‑negative integer
    quadruples (w,x,y,z) such that
        w^2 + x^2 + y^2 + z^2 == n   and
        w + 2*x + 3*y + 5*z          is a perfect square.   */

/* The helper below implements an integer square root for uint64_t
   values without using any external utilities.  It works for all
   values representable in idx_t (assumed to be uint64_t). */
static inline idx_t uint64_isqrt(idx_t v)
{
    idx_t r = 0;                    /* result */
    /* bit is the largest power of four <= v */
    idx_t bit = (idx_t)1 << 62;
    while (bit > v) bit >>= 2;
    while (bit)
    {
        if (v >= r + bit)
        {
            v -= r + bit;
            r = (r >> 1) + bit;
        }
        else
        {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;                       /* floor(sqrt(v)) */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Maximum coordinate value: floor(sqrt(n)) */
    idx_t max_coord = uint64_isqrt(n);

    seq_t count = (seq_t)0;

    for (idx_t w = 0; w <= max_coord; ++w)
    {
        idx_t w2 = w * w;
        if (w2 > n) break;

        for (idx_t x = 0; x <= max_coord; ++x)
        {
            idx_t wx = w2 + x * x;
            if (wx > n) break;

            for (idx_t y = 0; y <= max_coord; ++y)
            {
                idx_t wxy = wx + y * y;
                if (wxy > n) break;

                /* Remaining part that must be z^2 */
                idx_t rem = n - wxy;
                if (rem == 0)      /* z = 0 only */
                {
                    idx_t val = w + 2 * x + 3 * y;   /* +5*0 */
                    idx_t s = uint64_isqrt(val);
                    if (s * s == val) count += (seq_t)1;
                    continue;
                }

                /* z^2 must equal rem.  Compute z. */
                idx_t z = uint64_isqrt(rem);
                if (z * z != rem) continue;      /* rem not a square */

                idx_t val = w + 2 * x + 3 * y + 5 * z;
                idx_t s = uint64_isqrt(val);
                if (s * s == val) count += (seq_t)1;
            }
        }
    }
    return count;
}
