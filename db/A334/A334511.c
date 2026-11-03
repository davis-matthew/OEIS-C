/*
 * A334511  Eventual period of a single cell in rule 9 cellular automaton in a
 *          cyclic universe of width n.
 * https://oeis.org/A334511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3 - (-1)^n)*n/2 for n > 10; for n <= 10 the value is
 *            computed by a short simulation
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>

static seq_t simulate_period(idx_t n) {
    /* Only needed for n <= 10.  Use a fixed-size array for visited configs. */
    static int visited[1 << 10];
    int max_state = 1 << n;
    /* initialise visited to -1 only for the needed states */
    for (int i = 0; i < max_state; ++i) visited[i] = -1;

    int config = 1; /* single cell at position 0 set to 1 */
    int step = 0;
    while (1) {
        if (visited[config] != -1) {
            /* cycle length is (current_step - first_seen_step) */
            return (seq_t)(step - visited[config]);
        }
        visited[config] = step;
        /* compute next configuration */
        int next = 0;
        for (int i = 0; i < n; ++i) {
            int left  = (config >> ((i - 1 + n) % n)) & 1;
            int center = config >> i & 1;
            int right = (config >> ((i + 1) % n)) & 1;
            int pattern = (left << 2) | (center << 1) | right;
            int out = (9 >> pattern) & 1;  /* rule 9 */
            if (out) next |= (1 << i);
        }
        config = next;
        ++step;
    }
}

seq_t a(idx_t n) {
    if (n <= 10) {
        return simulate_period(n);
    } else {
        if ((n & 1ULL) == 0ULL) {
            return (seq_t)n;         /* even n: a(n) = n */
        } else {
            return ((seq_t)n) << 1; /* odd n: a(n) = 2n */
        }
    }
}
