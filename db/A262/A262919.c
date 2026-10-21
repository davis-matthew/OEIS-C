/*
 * A262919  Number of (5+1)X(n+1) 0..1 arrays with each row divisible by 3 and
 *          each column divisible by 7, read as a binary number with top and
 *          left being the most significant bits.
 * https://oeis.org/A262919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A262919 – Number of 6×(n+1) binary arrays.
 * Each row (as a binary number) is divisible by 3,
 * each column (as a 6‑bit binary number) is divisible by 7.
 *
 * The answer is computed by dynamic programming over the 10
 * admissible column patterns, tracking the sums of bits in
 * the even‑ and odd‑position columns modulo 3.
 * Formula (not a simple algebraic one):
 *     a(n) = Σ_s f_{ceil((n+1)/2)}(s) * f_{floor((n+1)/2)}(s)
 * where f_k(s) is the number of length‑k sequences of column
 * patterns whose weighted sum of bits equals the 6‑vector s
 * (mod 3).
 */

#include <stddef.h>

/* Pre‑compiled pattern values (decimal) that are ≡ 0 (mod 7) in 6 bits */
static const int pattern_vals[10] = {
    0, 7, 14, 21, 28, 35, 42, 49, 56, 63
};

/* Encoded number of states: 3^6 = 729 */
#define STATE_COUNT 729

/* Powers of 3 for state encoding/decoding */
static const int pow3[6] = {243, 81, 27, 9, 3, 1};

/* Transition table: next_state[s][p] gives the state after adding pattern p to state s */
static int next_state[STATE_COUNT][10];

/* Build the transition table once */
static void build_transition(void)
{
    for (int s = 0; s < STATE_COUNT; ++s) {
        int digits[6];
        int t = s;
        for (int i = 5; i >= 0; --i) {
            digits[i] = t % 3;
            t /= 3;
        }
        for (int p = 0; p < 10; ++p) {
            int val = pattern_vals[p];
            int new_s = 0;
            for (int i = 0; i < 6; ++i) {
                int bit = (val >> (5 - i)) & 1;
                int nd = (digits[i] + bit) % 3;
                new_s += nd * pow3[i];
            }
            next_state[s][p] = new_s;
        }
    }
}

/* Core counting routine */
static seq_t count_corresponding(int cols)
{
    int a = (cols + 1) / 2;      /* ceil(cols/2)  – positive‑sign columns */
    int b = cols / 2;            /* floor(cols/2) – negative‑sign columns */
    int max_k = a > b ? a : b;   /* largest of a, b */

    static int transition_built = 0;
    if (!transition_built) {
        build_transition();
        transition_built = 1;
    }

    seq_t dp[STATE_COUNT]   = {0};
    seq_t next[STATE_COUNT] = {0};
    seq_t dp_a[STATE_COUNT] = {0};
    seq_t dp_b[STATE_COUNT] = {0};

    dp[0] = (seq_t)1;  /* empty sequence has sum (0,…,0) */

    for (int step = 1; step <= max_k; ++step) {
        /* zero next array */
        for (int i = 0; i < STATE_COUNT; ++i) next[i] = 0;

        /* transition */
        for (int s = 0; s < STATE_COUNT; ++s) {
            if (dp[s] == (seq_t)0) continue;
            seq_t cur = dp[s];
            for (int p = 0; p < 10; ++p) {
                int ns = next_state[s][p];
                next[ns] = next[ns] + cur;
            }
        }

        /* copy next to dp */
        for (int i = 0; i < STATE_COUNT; ++i) dp[i] = next[i];

        if (step == a) {
            for (int i = 0; i < STATE_COUNT; ++i) dp_a[i] = dp[i];
        }
        if (step == b) {
            for (int i = 0; i < STATE_COUNT; ++i) dp_b[i] = dp[i];
        }
    }

    /* compute final answer */
    seq_t ans = 0;
    for (int i = 0; i < STATE_COUNT; ++i) {
        ans = ans + (dp_a[i] * dp_b[i]);
    }

    return ans;
}

/* Public function required by the build */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for 0, return 0 */

    /* The matrix has 6 rows and n+1 columns */
    int cols = (int)(n + 1);
    return count_corresponding(cols);
}
