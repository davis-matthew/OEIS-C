/*
 * A317769  Number of nX4 0..1 arrays with every element unequal to 0, 1 or 3
 *          horizontally, vertically or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A317769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 45, u64 n <= 90, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A317769
 * Number of n×4 0..1 arrays with upper left element zero and no
 * horizontal, vertical or anti‑diagonal adjacency equal to 0, 1 or 3.
 * Empirical recurrence (valid for n>23):
 *   a(n) = a(n-1) + 2*a(n-4) + 6*a(n-7)
 *          - a(n-8) - a(n-9) - a(n-10)
 *          - 7*a(n-11) - 7*a(n-12)
 *          + 2*a(n-13) - a(n-14)
 * Initial values:
 *   a(1)=5  a(2)=7  a(3)=11  a(4)=10  a(5)=14
 *   a(6)=23 a(7)=32 a(8)=55 a(9)=86 a(10)=143
 *   a(11)=226 a(12)=367 a(13)=599 a(14)=967 a(15)=1581
 *   a(16)=2574 a(17)=4220 a(18)=6868 a(19)=11207 a(20)=18264
 *   a(21)=29787 a(22)=48612 a(23)=79373
 */

seq_t a(idx_t n)
{
    /* initial values for n = 1 … 23 */
    static const seq_t init[23] = {
        5, 7, 11, 10, 14,
        23, 32, 55, 86, 143,
        226, 367, 599, 967, 1581,
        2574, 4220, 6868, 11207, 18264,
        29787, 48612, 79373
    };

    if (n <= 23) return init[n-1];

    /* cache up to a reasonable maximum (n ≈ 10 000 in typical tests) */
    static seq_t cache[10001];       /* cache[1] = a(1), ... */
    static idx_t cache_len = 0;      /* last computed index (0 if none) */

    /* initialise cache with known values */
    if (cache_len == 0) {
        for (idx_t i = 1; i <= 23; ++i) cache[i] = init[i-1];
        cache_len = 23;
    }

    /* return previously computed value if available */
    if (n <= cache_len) return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t val = cache[i-1];
        val += (seq_t)2 * cache[i-4];
        val += (seq_t)6 * cache[i-7];
        val -= cache[i-8];
        val -= cache[i-9];
        val -= cache[i-10];
        val -= (seq_t)7 * cache[i-11];
        val -= (seq_t)7 * cache[i-12];
        val += (seq_t)2 * cache[i-13];
        val -= cache[i-14];
        cache[i] = val;
    }
    cache_len = n;
    return cache[n];
}
