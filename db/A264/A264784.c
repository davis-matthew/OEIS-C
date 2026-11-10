/*
 * A264784  Run lengths of zeros in A265158.
 * https://oeis.org/A264784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the n-th maximal run of zeros in A265158, where
 *            A265158(1) = 1, A265158(2k) = 2*A265158(k), A265158(2k+1) =
 *            floor(A265158(k)/2). Method: A265158(m) is 0 or a power of 2.
 *            After the leading 1 bit, each 0 bit adds 1 to the exponent and
 *            each 1 bit subtracts 1. The value is 0 once the...
 * Range:     u8 n <= 31, u16 n <= 2681, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9755 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A264784_cur;
static idx_t A264784_cnt;
static idx_t A264784_target;
static seq_t A264784_res;
static int A264784_found;

static void A264784_dfs(unsigned m, unsigned d, unsigned e)
{
    if (A264784_found) return;
    if (d == m) {
        if (A264784_cur > 0) {
            A264784_cnt++;
            if (A264784_cnt == A264784_target) {
                A264784_res = A264784_cur;
                A264784_found = 1;
            }
            A264784_cur = 0;
        }
        return;
    }
    A264784_dfs(m, d + 1, e + 1);
    if (A264784_found) return;
    if (e > 0) {
        A264784_dfs(m, d + 1, e - 1);
    } else {
        unsigned r = m - d - 1;
        A264784_cur += ((seq_t)1) << r;
    }
}

seq_t a(idx_t n)
{
    unsigned m;
    A264784_cnt = 0;
    A264784_target = n;
    A264784_found = 0;
    A264784_res = 0;
    for (m = 1; m < 200 && !A264784_found; m++) {
        A264784_cur = 0;
        A264784_dfs(m, 0, 0);
        if (!A264784_found && A264784_cur > 0) {
            A264784_cnt++;
            if (A264784_cnt == A264784_target) {
                A264784_res = A264784_cur;
                A264784_found = 1;
            }
            A264784_cur = 0;
        }
    }
    return A264784_res;
}
