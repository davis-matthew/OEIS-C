/*
 * A250860  Number of (n+2)X(n+2) 0..3 arrays with nondecreasing sum of every
 *          three consecutive values in every row and column.
 * https://oeis.org/A250860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{p=0}^{2}∏_{q=0}^{2} F( ceil((n+2-p)/3),
 *            ceil((n+2-q)/3) ), where for positive integers R,C F(R,C) =
 *            ∏_{i=1}^{R}∏_{j=1}^{C} (i+j+2)/(i+j-1) = ∏_{s=2}^{R+C}
 *            ((s+2)/(s-1))^{cnt(s)}, cnt(s) = number of pairs (i,j) with
 *            i+j=s, = { s-1 if s ≤ min(R,C)+1 , min(R,C) if min(R,C)+1 < s ≤
 *            max(R,C)...
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 5, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define MAXV 7000          /* enough for n up to at least 10000   */
#define MAXP 1000          /* maximal number of primes ≤ MAXV  */

static int primes[MAXP];
static int prime_cnt = 0;
static int spf[MAXV+1];
static int prime_index[MAXV+1];
static int init_done = 0;

/* Initialize sieve and small prime factor array */
static void init(void)
{
    if (init_done) return;
    int i, j;
    for (i = 0; i <= MAXV; ++i) {
        spf[i] = 0;
        prime_index[i] = -1;
    }
    for (i = 2; i <= MAXV; ++i) {
        if (!spf[i]) {
            spf[i] = i;
            prime_index[i] = prime_cnt;
            primes[prime_cnt++] = i;
        }
        for (j = 0; j < prime_cnt && i * primes[j] <= MAXV; ++j) {
            int v = i * primes[j];
            spf[v] = primes[j];
            if (i % primes[j] == 0) break;
        }
    }
    init_done = 1;
}

/* Add the prime factorisation of x (sign = +1 or -1) multiplied by multiplier 'mult' to exp[] */
static void add_factor(int x, int sign, int mult, int exp[])
{
    while (x > 1) {
        int p = spf[x];
        int idx = prime_index[p];
        int cnt = 0;
        while (x % p == 0) {
            x /= p;
            ++cnt;
        }
        exp[idx] += sign * cnt * mult;
    }
}

/* Add the contributions of a single subgrid of size R x C */
static void add_subgrid(int R, int C, int exp[])
{
    if (R == 0 || C == 0) return;
    int a = R < C ? R : C;
    int b = R < C ? C : R;
    int smax = R + C;
    for (int s = 2; s <= smax; ++s) {
        int cnt;
        if (s <= a + 1) cnt = s - 1;
        else if (s <= b + 1) cnt = a;
        else cnt = R + C - s + 1;
        add_factor(s + 2, +1, cnt, exp); /* numerator */
        add_factor(s - 1, -1, cnt, exp); /* denominator */
    }
}

seq_t a(idx_t n)
{
    init();

    idx_t m = n + 2;               /* size of the array: (n+2) × (n+2) */
    int rows[3], cols[3];
    for (int t = 0; t < 3; ++t) {
        rows[t] = (int)((m - 1 - t) / 3) + 1;
        cols[t] = (int)((m - 1 - t) / 3) + 1;
    }

    int exp[MAXP];
    for (int i = 0; i < prime_cnt; ++i) exp[i] = 0;

    for (int p = 0; p < 3; ++p) {
        for (int q = 0; q < 3; ++q) {
            add_subgrid(rows[p], cols[q], exp);
        }
    }

    seq_t result = 1;
    for (int i = 0; i < prime_cnt; ++i) {
        int e = exp[i];
        if (e > 0) {
            seq_t power = 1;
            seq_t cur = (seq_t)primes[i];
            int ee = e;
            while (ee > 0) {
                if (ee & 1) power = power * cur;
                cur = cur * cur;
                ee >>= 1;
            }
            result = result * power;
        }
    }
    return result;
}
