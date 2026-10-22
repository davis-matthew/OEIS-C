/*
 * A371410  Row sums of A371409: sums of the positions of right parentheses in
 *          the properly nested string of parentheses encoded by A063171(n).
 * https://oeis.org/A371410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of positions of right parentheses in the nth Dyck
 *            word when Dyck words are ordered by the binary value of their
 *            '('=1, ')'=0 encoding
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define MAX_LEN 256   /* safe upper bound for the problem limits */

seq_t a(idx_t n) {
    /* Find the length group 2k that contains the nth Dyck word. */
    idx_t k = 1;
    idx_t cat = 1;            /* Catalan(k) */
    idx_t cumulative = 1;     /* sum_{i=1..k} C_i */

    while (cumulative < n) {
        ++k;
        cat = cat * ((idx_t)4 * k - 2) / (k + 1);  /* C_k = C_{k-1}*(4k-2)/(k+1) */
        cumulative += cat;
    }

    /* 0‑based rank of the word within its length. */
    idx_t rank = n - (cumulative - cat) - 1;

    idx_t len = 2 * k;            /* number of bits in the word */

    /* Precompute DP table: dp[rem][bal] = number of completions
       of length rem with current balance bal. */
    static idx_t dp[MAX_LEN + 1][MAX_LEN + 1];

    for (idx_t bal = 0; bal <= len; ++bal)
        dp[0][bal] = (bal == 0 ? 1 : 0);

    for (idx_t rem = 1; rem <= len; ++rem) {
        for (idx_t bal = 0; bal <= len; ++bal) {
            idx_t val = 0;
            if (bal + 1 <= len)
                val += dp[rem - 1][bal + 1];   /* place '(' */
            if (bal > 0)
                val += dp[rem - 1][bal - 1];   /* place ')' */
            dp[rem][bal] = val;
        }
    }

    /* Build the word bit by bit, accumulating the sum of positions of ')' */
    seq_t sum = 0;
    idx_t bal = 1;                /* first bit is always '(' */
    /* position 1 is already accounted for (it is a '('), so start at 2 */
    for (idx_t pos = 2; pos <= len; ++pos) {
        idx_t rem = len - pos;
        idx_t countZero = 0;
        if (bal > 0)
            countZero = dp[rem][bal - 1];

        if (rank < countZero) {          /* choose ')' */
            sum += (seq_t)pos;
            --bal;
        } else {                          /* choose '(' */
            rank -= countZero;
            ++bal;
        }
    }

    return sum;
}
