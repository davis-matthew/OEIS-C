/*
 * A248444  Number of length n+5 0..4 arrays with no three disjoint pairs in
 *          any consecutive six terms having the same sum.
 * https://oeis.org/A248444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of length n+5 0..4 arrays with no three disjoint pairs in
 *            any consecutive six terms having the same sum. The
 *            implementation uses a finite‑state automaton: each state is a
 *            5‑digit block; transitions are allowed only if the new 6‑tuple
 *            does not contain a forbidden pattern. The transition tab...
 * Range:     u8 none, u16 n <= 2, u32 n <= 9, u64 n <= 24, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Pre‑computation helpers --------------------------------------------------- */
#define STATE_COUNT 3125   /* 5^5 */
#define MAX_DIGIT   4
#define POW5_4      625     /* 5^4 */

static uint8_t allowed[STATE_COUNT][5];   /* allowed[old_state][new_digit] */

/* All 15 perfect matchings of 6 indices into 3 disjoint pairs.          */
static int pairings[15][3][2];
static int pair_count = 0;

/* Generate all matchings recursively. */
static void generate_matchings(int n, int depth, int *used, int (*current)[2]) {
    int i;
    for (i = 0; i < n; ++i) if (!used[i]) break;
    if (i == n) {                /* all paired */
        for (int d = 0; d < 3; ++d)
            for (int k = 0; k < 2; ++k)
                pairings[pair_count][d][k] = current[d][k];
        ++pair_count;
        return;
    }
    used[i] = 1;
    for (int j = i + 1; j < n; ++j) if (!used[j]) {
        used[j] = 1;
        current[depth][0] = i;
        current[depth][1] = j;
        generate_matchings(n, depth + 1, used, current);
        used[j] = 0;
    }
    used[i] = 0;
}

static void init_pairings(void) {
    int used[6] = {0};
    int curpair[3][2];
    generate_matchings(6, 0, used, curpair);
    /* pair_count should be 15 */
}

/* Test whether the 6‑tuple a[0..5] contains a forbidden
   disjoint triple of pairs with equal sums.             */
static bool forbidden(const int a[6]) {
    for (int p = 0; p < pair_count; ++p) {
        int s0 = a[pairings[p][0][0]] + a[pairings[p][0][1]];
        int s1 = a[pairings[p][1][0]] + a[pairings[p][1][1]];
        int s2 = a[pairings[p][2][0]] + a[pairings[p][2][1]];
        if (s0 == s1 && s1 == s2) return true;
    }
    return false;
}

static int init_allowed_once = 0;

static void init_allowed(void) {
    if (init_allowed_once) return;
    init_pairings();
    for (int idx = 0; idx < STATE_COUNT; ++idx) {
        int tmp = idx;
        int d[5];
        for (int pos = 4; pos >= 0; --pos) {
            d[pos] = tmp % 5;
            tmp /= 5;
        }
        for (int f = 0; f <= MAX_DIGIT; ++f) {
            int a[6] = { d[0], d[1], d[2], d[3], d[4], f };
            allowed[idx][f] = forbidden(a) ? 0 : 1;
        }
    }
    init_allowed_once = 1;
}

/* --------------------------------------------------------------------------- */

/* Expected function signature. */
seq_t a(idx_t n)
{
    /* Ensure transition table is ready. */
    init_allowed();

    /* Dynamic programming arrays.  The state is a 5‑digit block. */
    static seq_t dp[2][STATE_COUNT];
    int cur = 0, nxt = 1;

    /* Initialise current DP: all 5‑digit sequences are allowed. */
    for (int i = 0; i < STATE_COUNT; ++i) {
        dp[cur][i] = 0;
        dp[nxt][i] = 0;
    }
    for (int i = 0; i < STATE_COUNT; ++i) dp[cur][i] = 1;

    /* Perform 'n' extensions (we start from length 5). */
    for (idx_t step = 0; step < n; ++step) {
        /* Zero the next array. */
        for (int i = 0; i < STATE_COUNT; ++i) dp[nxt][i] = 0;

        /* Transition. */
        for (int old_state = 0; old_state < STATE_COUNT; ++old_state) {
            seq_t ways = dp[cur][old_state];
            if (ways == 0) continue;
            for (int d = 0; d <= MAX_DIGIT; ++d) {
                if (!allowed[old_state][d]) continue;
                int new_state = (old_state % POW5_4) * 5 + d;
                dp[nxt][new_state] += ways;
            }
        }
        /* Swap. */
        int tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* Sum all final states to obtain the answer. */
    seq_t total = 0;
    for (int i = 0; i < STATE_COUNT; ++i)
        total += dp[cur][i];

    return total;
}
