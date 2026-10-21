/*
 * A253700  Number of (3+1)X(n+1) 0..1 arrays with every 2X2 subblock ne-sw
 *          antidiagonal difference unequal to its neighbors horizontally and
 *          nw+se diagonal sum unequal to its neighbors vertically.
 * https://oeis.org/A253700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-2)+2*a(n-4)+2*a(n-5)-3*a(n-6)+a(n-7)-a(n-8)
 *            -6*a(n-9)+2*a(n-10)-2*a(n-12)-3*a(n-13)+a(n-14)
 *            +6*a(n-15)+a(n-16)+a(n-17)-a(n-18)-a(n-19) (n>21) Initial
 *            values: a(1)=104, a(2)=146, a(3)=234, a(4)=356, a(5)=618,
 *            a(6)=968, a(7)=1608, a(8)=2558, a(9)=4278, a(10)=6854,
 *            a(11)=11266, a(12)=18...
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 37, u64 n <= 82, u128 n <= 173,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

static const seq_t init_vals[21] = {
    104, 146, 234, 356, 618,
    968, 1608, 2558, 4278, 6854,
    11266, 18192, 29910, 48348, 79116,
    128376, 209934, 340652, 556268, 904160, 1475412
};

static seq_t memo[10001];              /* indices 1..10000 */
static bool memo_ready[10001] = {false};
static idx_t memo_max = 0;              /* highest computed index */

/* compute values up to n (inclusive), n <= 10000 */
static void ensure(idx_t n)
{
    if (n <= memo_max) return;
    if (n > 10000) return;   /* out‑of‑bounds for static array – caller must avoid */

    for (idx_t i = memo_max + 1; i <= n; ++i) {
        if (i <= 21) {
            memo[i] = init_vals[i - 1];
            memo_ready[i] = true;
        } else {
            seq_t pos = 0, neg = 0;

            pos += memo[i - 2] + memo[i - 2];
            pos += memo[i - 4] + memo[i - 4];
            pos += memo[i - 5] + memo[i - 5];
            neg += memo[i - 6];
            neg += memo[i - 6];
            neg += memo[i - 6];
            pos += memo[i - 7];
            neg += memo[i - 8];
            neg += memo[i - 9];
            neg += memo[i - 9];
            neg += memo[i - 9];
            neg += memo[i - 9];
            neg += memo[i - 9];
            neg += memo[i - 9];
            pos += memo[i - 10] + memo[i - 10];
            neg += memo[i - 12];
            neg += memo[i - 12];
            neg += memo[i - 13];
            neg += memo[i - 13];
            neg += memo[i - 13];
            pos += memo[i - 14];
            pos += memo[i - 15];
            pos += memo[i - 15];
            pos += memo[i - 15];
            pos += memo[i - 15];
            pos += memo[i - 15];
            pos += memo[i - 15];
            pos += memo[i - 16];
            pos += memo[i - 17];
            neg += memo[i - 18];
            neg += memo[i - 19];

            memo[i] = pos - neg;
            memo_ready[i] = true;
        }
    }
    memo_max = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;        /* undefined for offset 1 */
    ensure(n);
    return memo[n];
}
