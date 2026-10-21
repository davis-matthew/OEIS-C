/*
 * A298812  Growth series for group with presentation < S, T : S^2 = T^3 =
 *          (S*T)^10 = 1 >.
 * https://oeis.org/A298812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-4) + 2*a(n-6) + 4*a(n-8) + 2*a(n-10) + a(n-12) -
 *            a(n-16) + p(n) where p(n) is the coefficient of x^n in the
 *            numerator of the generating function of sequence A298812: p =
 *            {1,3,4,6,7,9,10,12,12,12,12,9,9,6,6,3,3,0,-2}. Initial values:
 *            a(0)=1, a(1)=3, a(2)=4, a(3)=6, a(4)=8, a(5)=12, a(6)...
 * Range:     u8 n <= 14, u16 n <= 32, u32 n <= 66, u64 n <= 137,
 *            u128 n <= 277, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;
static size_t a_capacity = 0;
static idx_t a_last = (idx_t)-1;

/* Coefficients of the numerator (p(n)) for n = 0..18 */
static const int p_coeff[19] = {
    1, 3, 4, 6, 7, 9,10,12,12,12,12,9,9,6,6,3,3,0,-2
};

/* Ensure the cache can hold indices up to n (inclusive). */
static void a_ensure_capacity(idx_t n)
{
    size_t required = (size_t)(n + 1);
    if (required <= a_capacity)
        return;
    size_t new_cap = a_capacity ? a_capacity * 2 : 32;
    while (new_cap < required)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failed */
    a_cache = tmp;
    a_capacity = new_cap;
}

/* Initialisation on first call. */
static void a_init(void)
{
    if (a_cache != NULL)
        return;
    a_ensure_capacity(16);      /* at least 17 entries for 0..16 */
    static const seq_t init[17] = {
        1, 3, 4, 6, 8, 12, 16, 24, 32, 48, 62, 87, 114, 165, 216, 312, 408
    };
    for (size_t i = 0; i < 17; ++i)
        a_cache[i] = init[i];
    a_last = 16;
}

/* Compute a(n) for any n >= 0. */
seq_t a(idx_t n)
{
    a_init();
    if (n <= a_last)
        return a_cache[n];

    /* Grow the table up to n */
    a_ensure_capacity(n);
    for (idx_t i = a_last + 1; i <= n; ++i) {
        seq_t val = a_cache[i-4];
        val += (seq_t)2 * a_cache[i-6];
        val += (seq_t)4 * a_cache[i-8];
        val += (seq_t)2 * a_cache[i-10];
        val += a_cache[i-12];
        val -= a_cache[i-16];

        if (i <= 18) {
            int d = p_coeff[i];
            if (d >= 0)
                val += (seq_t)d;
            else
                val -= (seq_t)(-d);
        }
        a_cache[i] = val;
    }
    a_last = n;
    return a_cache[n];
}
