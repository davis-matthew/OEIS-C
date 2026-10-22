/*
 * A365781  Lexicographically earliest permutation of the composite numbers
 *          such that a(n) - [the next digit] is a prime number.
 * https://oeis.org/A365781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base, fini, full
 * Formula:   a(n) is the lexicographically earliest permutation of the *
 *            composite numbers such that for every k ≥ 1, a(k) - fd(a(k+1))
 *            is a * prime, where fd(x) is the first decimal digit of x. * The
 *            sequence is constructed greedily: start with the smallest
 *            composite * (4) and repeatedly append the smallest...
 * Range:     u8 n <= 1, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required headers.  seq_t and idx_t are provided by the build system. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* --------------------------------------------------------------------- */
/* Helper: first decimal digit of a number                                 */
static seq_t first_digit(seq_t x)
{
    while (x >= (seq_t)10) {
        x = x / (seq_t)10;
    }
    return x;
}

/* Helper: deterministic primality test for small integers (≤ 20000). */
static bool is_prime_int(int v)
{
    if (v < 2) return false;
    if (v % 2 == 0) return v == 2;
    for (int d = 3; d * d <= v; d += 2) {
        if (v % d == 0) return false;
    }
    return true;
}

/* --------------------------------------------------------------------- */
/* Main function expected by the tests.  It lazily builds the sequence
 * once and returns the requested term. */
seq_t a(idx_t n)
{
    /* Limits for generating composites.  These are large enough to
     * contain all terms needed for the sequence (the last term is 900). */
    enum { COMPOSITE_LIMIT = 2000, INITIAL_COMP_CAP = 256, INITIAL_SEQ_CAP = 128 };

    /* Static state, preserved between calls. */
    static seq_t *composites = NULL;
    static bool *used_composites = NULL;
    static size_t comp_len = 0, comp_cap = 0;

    static seq_t *seq = NULL;
    static size_t seq_len = 0, seq_cap = 0;

    /* Build state on first call. */
    if (!composites) {
        /* Allocate space for composites. */
        comp_cap = INITIAL_COMP_CAP;
        composites = (seq_t *)malloc(comp_cap * sizeof(seq_t));
        used_composites = (bool *)malloc(comp_cap * sizeof(bool));
        if (!composites || !used_composites) exit(1);

        /* Generate all composite numbers up to COMPOSITE_LIMIT. */
        for (int i = 4; i <= COMPOSITE_LIMIT; ++i) {
            if (!is_prime_int(i)) {
                if (comp_len == comp_cap) {
                    comp_cap <<= 1;
                    composites = (seq_t *)realloc(composites, comp_cap * sizeof(seq_t));
                    used_composites = (bool *)realloc(used_composites, comp_cap * sizeof(bool));
                    if (!composites || !used_composites) exit(1);
                }
                composites[comp_len++] = (seq_t)i;
                used_composites[comp_len-1] = false;
            }
        }
    }

    if (!seq) {
        seq_cap = INITIAL_SEQ_CAP;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) exit(1);

        /* Construct the lexicographically earliest sequence. */
        if (comp_len > 0) {
            seq_len = 1;
            seq[0] = composites[0];
            used_composites[0] = true;

            while (true) {
                bool found = false;
                for (size_t j = 0; j < comp_len; ++j) {
                    if (used_composites[j]) continue;

                    seq_t last = seq[seq_len - 1];
                    seq_t fd = first_digit(composites[j]);
                    seq_t diff_seq = last - fd;
                    if (diff_seq <= 0) continue;

                    /* diff_seq is tiny (≤ 900), so safe to cast. */
                    unsigned long long diff = (unsigned long long)diff_seq;
                    if (!is_prime_int((int)diff)) continue;

                    /* Append this composite. */
                    if (seq_len == seq_cap) {
                        seq_cap <<= 1;
                        seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                        if (!seq) exit(1);
                    }
                    seq[seq_len++] = composites[j];
                    used_composites[j] = true;
                    found = true;
                    break;
                }
                if (!found) break;   /* no further composite can be appended */
            }
        }
    }

    /* Return the requested term. */
    if (n == 0 || (idx_t)n > seq_len) return (seq_t)0;
    return seq[(size_t)(n - 1)];
}
