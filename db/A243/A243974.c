/*
 * A243974  Integers j not of form 3m+1 such that for any integer k>0,
 *          j*10^k-1 has a divisor in the set { 7, 11, 13, 37 }.
 * https://oeis.org/A243974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-24) + 111111 for n > 24. First 24 terms: 10176,
 *            17601, 19361, 25827, 27147, 27686, 35916, 36048, 45462, 47213,
 *            48036, 49248, 54638, 62864, 64184, 64899, 72953, 73085, 82499,
 *            85073, 86285, 93435, 101760, 101936. The function computes these
 *            terms by testing the covering set condition ove...
 * Range:     u8 none, u16 n <= 16, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static seq_t base[24];
static bool base_inited = false;

static void init_base(void)
{
    const int ds[4] = {7, 11, 13, 37};
    const int num_ds = 4;
    int pow10_mod[4][6];
    for (int i = 0; i < num_ds; ++i) {
        int d = ds[i];
        pow10_mod[i][0] = 1 % d;
        for (int r = 1; r < 6; ++r)
            pow10_mod[i][r] = (pow10_mod[i][r - 1] * 10) % d;
    }

    idx_t count = 0;
    for (idx_t j = 1; j < 111111; ++j) {
        if (j % 3 == 1)          /* skip numbers of the form 3m+1 */
            continue;

        bool ok_all = true;
        for (int r = 0; r < 6 && ok_all; ++r) {
            bool ok_r = false;
            for (int di = 0; di < num_ds; ++di) {
                int d  = ds[di];
                int jm = (int)(j % d);                     /* j mod d */
                if ((jm * pow10_mod[di][r]) % d == 1) {   /* j*10^r ≡ 1 (mod d) */
                    ok_r = true;
                    break;
                }
            }
            if (!ok_r)
                ok_all = false;
        }

        if (ok_all) {
            base[count++] = (seq_t)j;
            if (count == 24)
                break;
        }
    }
    base_inited = true;
}

seq_t a(idx_t n)
{
    if (!base_inited)
        init_base();

    idx_t period = (n - 1) / 24;   /* how many full periods before n */
    idx_t pos    = (n - 1) % 24;   /* index inside the period */

    const seq_t PERIOD = (seq_t)111111;   /* 3 × 7 × 11 × 13 × 37 */
    return base[pos] + (seq_t)period * PERIOD;
}
