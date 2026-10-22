/*
 * A365533  a(n) is the nim-value of the SALIQUANT game where the option is to
 *          subtract a nondivisor from 2*n.
 * https://oeis.org/A365533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = SG(2*n) * SG(n) = mex{ SG(n-k) : 1 <= k <= n and k is NOT
 *            a divisor of n } * For odd n, SG(n) = (n-1)/2. (See OEIS A365533
 *            definition.)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* for uint64_t etc. */

/* static buffers ------------------------------------------------------- */
/* Maximum 2*n needed for n up to 10 000 -> 20 000.  Allocate a little room. */
#define MAX_SG 20001

static seq_t sg[MAX_SG] = {0};    /* Grundy numbers, indexed by n (0…MAX_SG-1) */
static size_t sg_computed = 0;    /* highest n already computed (0..sg_computed) */

static int mark[MAX_SG] = {0};    /* used for mex calculation with a stamp trick */
static int cur_mark = 1;          /* stamp value, increments for each even n */

/* --------------------------------------------------------------------- */
static void compute_sg(size_t max_n)
{
    /* Clamp max_n to the pre-allocated buffer size.  In the intended
       range (2*n ≤ 20 000) the clamp never takes effect. */
    if (max_n >= MAX_SG)
        max_n = MAX_SG - 1;

    if (max_n <= sg_computed)
        return;

    for (size_t n = sg_computed + 1; n <= max_n; ++n)
    {
        if ((n & 1u) != 0)                     /* odd n */
        {
            sg[n] = (seq_t)((n - 1u) / 2u);
        }
        else                                   /* even n */
        {
            int stamp = cur_mark++;
            if (stamp == 0x7FFFFFFF)          /* avoid int overflow */
            {
                for (size_t i = 0; i < MAX_SG; ++i)
                    mark[i] = 0;
                stamp = 1;
                cur_mark = 2;
            }

            /* Mark Grundy values reachable by a legal subtraction. */
            for (size_t k = 1; k < n; ++k)
            {
                if (n % k == 0)                /* k is a divisor – skip */
                    continue;
                seq_t val = sg[n - k];
                size_t idx = (size_t)val;
                if (idx < MAX_SG)
                    mark[idx] = stamp;
            }

            /* Find mex: smallest non‑negative integer not in the mark set. */
            size_t mex = 0;
            while (mex < MAX_SG && mark[mex] == stamp)
                ++mex;
            sg[n] = (seq_t)mex;
        }
    }
    sg_computed = max_n;
}

/* --------------------------------------------------------------------- */
/* Public API required by the OEIS harness. */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset starts at 1 – 0 is undefined, return 0 safely */
        return 0;

    size_t target = (size_t)n * 2u;   /* we need SG(2*n) */
    compute_sg(target);
    return sg[target];
}
