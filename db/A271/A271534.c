/*
 * A271534  Numbers n with digits abc..xyz such that n = (a + b + c + ... + x
 *          + y + z) + (a^b + b^c + c^d + ... + x^y + y^z).
 * https://oeis.org/A271534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) is the nth integer that satisfies n = digit_sum(n) +
 *            sum_{i} digit_i ^ digit_{i+1} (adjacent-digit powers). The
 *            sequence is finite; the known terms up to n = 12 are
 *            1,2,3,4,5,6,7,8,9,33,92,1683552
 * Range:     u8 n <= 10, u16 n <= 11, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdint.h>   /* for uint8_t if needed */

static seq_t powtab[10][10];   /* a^b for digits 0..9, 0^0 = 1 */

static void init_powtab(void)
{
    static int done = 0;
    if (done) return;
    done = 1;
    for (int a = 0; a <= 9; ++a) {
        for (int b = 0; b <= 9; ++b) {
            seq_t val;
            if (b == 0)
                val = 1;                  /* a^0 = 1, including 0^0 */
            else {
                seq_t base = a;
                for (int k = 1; k < b; ++k)
                    base *= (seq_t)a;    /* multiply b-1 times */
                val = base;
            }
            powtab[a][b] = val;
        }
    }
}

static seq_t cache[12];
static size_t cache_len = 0;

static int is_term(idx_t num)
{
    /* digits in most significant order */
    int d[16];
    int len = 0;
    idx_t tmp = num;
    if (tmp == 0) {
        d[len++] = 0;
    } else {
        while (tmp > 0) {
            d[len++] = tmp % 10;
            tmp /= 10;
        }
        /* reverse to get MSB first */
        for (int i = 0; i < len / 2; ++i) {
            int t = d[i];
            d[i] = d[len - 1 - i];
            d[len - 1 - i] = t;
        }
    }

    /* sum of digits */
    seq_t sum_digits = 0;
    for (int i = 0; i < len; ++i)
        sum_digits += (seq_t)d[i];

    /* sum of adjacent powers */
    seq_t sum_pows = 0;
    for (int i = 0; i + 1 < len; ++i)
        sum_pows += powtab[d[i]][d[i + 1]];

    seq_t rhs = sum_digits + sum_pows;
    return (seq_t)num == rhs;
}

static void precompute(void)
{
    init_powtab();
    /* largest term known: 1,683,552 */
    const idx_t LIMIT = 2000000;
    for (idx_t n = 1; n <= LIMIT && cache_len < 12; ++n) {
        if (is_term(n))
            cache[cache_len++] = (seq_t)n;
    }
}

seq_t a(idx_t n)
{
    if (cache_len == 0) precompute();
    if (n == 0 || n > cache_len) return 0;
    return cache[n - 1];
}
