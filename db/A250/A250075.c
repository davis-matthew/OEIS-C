/*
 * A250075  Number of length n+5 0..2 arrays with every six consecutive terms
 *          having the maximum of some two terms equal to the minimum of the
 *          remaining four terms.
 * https://oeis.org/A250075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 7, u32 n <= 21, u64 n <= 48, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A250075 – Compute the number of length n+5 arrays over {0,1,2}
 *  such that every six consecutive terms have the maximum of some two
 *  equal to the minimum of the remaining four.
 *
 *  The implementation builds a deterministic finite automaton (DFA)
 *  over the last five symbols, counts admissible words by dynamic
 *  programming, and caches results for all n ≥ 1.  No closed‑form
 *  expression is known, hence the comment explains that the method
 *  is a dynamic‑programming search.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/*  External integer types defined by the build system; do not declare them here.  */
/*  typedef idx_t;  code will compile with a proper definition elsewhere  */
/*  typedef seq_t;  code will compile with a proper definition elsewhere  */

static bool    good[243][3];      /* good[s][a] : does 6‑tuple from state s and a satisfy the condition? */
static int     next_state[243][3];/* next_state[s][a] : DFA transition */
static bool    tables_initialized = false;

/*  Precompute DFA tables – run once.  */
static void init_tables(void)
{
    for (int s = 0; s < 243; ++s) {            /* 3⁵ states */
        int digits[5];
        int tmp = s;
        for (int k = 4; k >= 0; --k) {         /* decode base‑3 representation */
            digits[k] = tmp % 3;
            tmp /= 3;
        }
        for (int a = 0; a < 3; ++a) {
            int t[6];
            for (int i = 0; i < 5; ++i) t[i] = digits[i];
            t[5] = a;

            bool ok = false;
            for (int i = 0; i < 5 && !ok; ++i)
                for (int j = i + 1; j < 6 && !ok; ++j) {
                    int maxij = (t[i] > t[j]) ? t[i] : t[j];
                    int minrest = 3;          /* greater than any allowed value (0..2) */
                    for (int k = 0; k < 6; ++k)
                        if (k != i && k != j && t[k] < minrest)
                            minrest = t[k];
                    if (maxij == minrest) ok = true;
                }
            good[s][a] = ok;

            next_state[s][a] = ((s % 81) * 3) + a;   /* shift left and append a  */
        }
    }
    tables_initialized = true;
}

/*  Dynamic‑programming tables for counting sequences of current length.  */
static seq_t dp_cur[243];
static seq_t dp_next[243];
static bool   dp_initialized   = false;

/*  Cache of results a(1), a(2), …  */
static seq_t *totals          = NULL;  /* dynamically reallocated */
static size_t totals_cap      = 0;     /* allocated capacity (≥1) */
static size_t totals_len      = 0;     /* number of valid entries (≥1) */
static idx_t  current_n_demo  = 0;     /* largest n for which totals[n] is valid */

/*  Ensure totals[needed] exists.  */
static void
ensure_totals_capacity(idx_t needed)
{
    size_t needed_cap = (size_t)needed + 1;
    if (needed_cap > totals_cap) {
        size_t new_cap = totals_cap ? totals_cap * 2 : 16;
        while (new_cap < needed_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(totals, new_cap * sizeof(seq_t));
        totals = tmp;
        totals_cap = new_cap;
    }
    if (needed_cap > totals_len) totals_len = needed_cap;
}

/*  Return a(n).  */
seq_t
a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n < 1 */

    /*  Initialise tables and DP once.  */
    if (!tables_initialized) init_tables();
    if (!dp_initialized) {
        for (int i = 0; i < 243; ++i) dp_cur[i] = 1;   /* all 5‑tuples are allowed initially */
        for (int i = 0; i < 243; ++i) dp_next[i] = 0;
        dp_initialized = true;
        current_n_demo = 0;
        totals = NULL;
        totals_cap = 0;
        totals_len = 0;
    }

    /*  Ensure cache large enough.  */
    ensure_totals_capacity(n);

    /*  Compute values up to required n, caching intermediate results.  */
    while (current_n_demo < n) {
        for (int i = 0; i < 243; ++i) dp_next[i] = 0;

        for (int s = 0; s < 243; ++s) {
            if (dp_cur[s] == 0) continue;
            for (int a = 0; a < 3; ++a) {
                if (!good[s][a]) continue;
                int ns = next_state[s][a];
                dp_next[ns] += dp_cur[s];
            }
        }

        /*  Move to next length.  */
        for (int i = 0; i < 243; ++i) dp_cur[i] = dp_next[i];

        /*  Sum all states to get total count for this n.  */
        seq_t sum = 0;
        for (int i = 0; i < 243; ++i) sum += dp_cur[i];

        idx_t step = current_n_demo + 1;
        totals[step] = sum;
        current_n_demo = step;
    }

    return totals[n];
}
