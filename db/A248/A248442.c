/*
 * A248442  Number of length n+5 0..2 arrays with no three disjoint pairs in
 *          any consecutive six terms having the same sum.
 * https://oeis.org/A248442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed via dynamic programming over a 5‑symbol state
 *            automaton * representing the last five entries of a 0–2
 *            sequence. The count equals the * number of length n+5 0–2 arrays
 *            that contain no consecutive six terms having * three disjoint
 *            pairs of equal sums
 * Range:     u8 none, u16 n <= 6, u32 n <= 19, u64 n <= 44, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
/* seq_t and idx_t are provided by the build. */

static bool banned_init = false;
static bool banned[729];

static const int pairings[15][3][2] = {
    {{0,1},{2,3},{4,5}},
    {{0,1},{2,4},{3,5}},
    {{0,1},{2,5},{3,4}},
    {{0,2},{1,3},{4,5}},
    {{0,2},{1,4},{3,5}},
    {{0,2},{1,5},{3,4}},
    {{0,3},{1,2},{4,5}},
    {{0,3},{1,4},{2,5}},
    {{0,3},{1,5},{2,4}},
    {{0,4},{1,2},{3,5}},
    {{0,4},{1,3},{2,5}},
    {{0,4},{1,5},{2,3}},
    {{0,5},{1,2},{3,4}},
    {{0,5},{1,3},{2,4}},
    {{0,5},{1,4},{2,3}}
};

static void init_banned(void)
{
    if (banned_init)
        return;
    for (int code = 0; code < 729; ++code) {
        int tmp = code;
        int a[6];
        /* decode base‑3 digits; a[0] is the most signficant corresponding
           to the first element of the 6‑tuple. */
        for (int i = 5; i >= 0; --i) {
            a[i] = tmp % 3;
            tmp /= 3;
        }
        bool bad = false;
        for (int p = 0; p < 15 && !bad; ++p) {
            int s0 = a[pairings[p][0][0]] + a[pairings[p][0][1]];
            int s1 = a[pairings[p][1][0]] + a[pairings[p][1][1]];
            int s2 = a[pairings[p][2][0]] + a[pairings[p][2][1]];
            if (s0 == s1 && s1 == s2)
                bad = true;
        }
        banned[code] = bad;
    }
    banned_init = true;
}

seq_t a(idx_t n)
{
    init_banned();
    if (n == 0)
        return 0;          /* safe value for out‑of‑range indices */

    const int STATES = 243;     /* 3^5 */
    seq_t dp[STATES];
    seq_t ndp[STATES];
    const seq_t zero = 0;

    /* initial vector: every 5‑tuple is allowed before any 6‑term window has
       appeared. */
    for (int i = 0; i < STATES; ++i)
        dp[i] = 1;

    /* we have to append n additional symbols to reach length n+5 */
    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < STATES; ++i)
            ndp[i] = 0;

        for (int s = 0; s < STATES; ++s) {
            if (dp[s] == zero)
                continue;
            for (int v = 0; v < 3; ++v) {
                if (!banned[s * 3 + v]) {
                    int next = (s % 81) * 3 + v;     /* shift left, drop oldest */
                    ndp[next] += dp[s];
                }
            }
        }
        for (int i = 0; i < STATES; ++i)
            dp[i] = ndp[i];
    }

    seq_t result = 0;
    for (int i = 0; i < STATES; ++i)
        result += dp[i];
    return result;
}
