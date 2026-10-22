/*
 * A305342  Number of nX3 0..1 arrays with every element unequal to 0, 1, 3, 5
 *          or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A305342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) +3*a(n-3) +2*a(n-4) +a(n-5) +a(n-6) -a(n-7)
 *            -a(n-8) -a(n-9) -a(n-11) -a(n-12) with initial values a(1)=3,
 *            a(2)=9, a(3)=14, a(4)=28, a(5)=63, a(6)=126, a(7)=245, a(8)=505,
 *            a(9)=1037, a(10)=2088, a(11)=4230, a(12)=8613
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 30, u64 n <= 61, u128 n <= 124,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t memo[10001];     /* capacity for n up to 10 000 */
static size_t computed_max = 12; /* index (0‑based) of largest computed term */
static int initialized = 0;

/* Initialise the first 12 terms.  Done once, lazily. */
static void init_base(void)
{
    if (initialized)
        return;
    memo[0] = (seq_t)3;
    memo[1] = (seq_t)9;
    memo[2] = (seq_t)14;
    memo[3] = (seq_t)28;
    memo[4] = (seq_t)63;
    memo[5] = (seq_t)126;
    memo[6] = (seq_t)245;
    memo[7] = (seq_t)505;
    memo[8] = (seq_t)1037;
    memo[9] = (seq_t)2088;
    memo[10] = (seq_t)4230;
    memo[11] = (seq_t)8613;
    /* computed_max already 12 (next index to compute) */
    initialized = 1;
}

/* Compute a(n).  n is 1‑based; n==0 returns 0 for safety. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;    /* undefined offset, return 0 */

    size_t target = (size_t)n - 1;   /* convert to 0‑based index */

    /* Ensure the base terms are filled. */
    init_base();

    /* Guard against n beyond the allocated capacity. */
    if (target >= sizeof(memo)/sizeof(memo[0]))
        abort();                 /* or handle error as desired */

    /* If we already have the value, return it directly. */
    if (target < computed_max)
        return memo[target];

    /* Compute missing terms up to target. */
    for (size_t i = computed_max; i <= target; ++i)
    {
        seq_t val =   memo[i - 1]
                    + (seq_t)3 * memo[i - 3]
                    + (seq_t)2 * memo[i - 4]
                    + memo[i - 5]
                    + memo[i - 6]
                    - memo[i - 7]
                    - memo[i - 8]
                    - memo[i - 9]
                    - memo[i - 11]
                    - memo[i - 12];
        memo[i] = val;
    }
    computed_max = target + 1;   /* next index to compute */
    return memo[target];
}
