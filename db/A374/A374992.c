/*
 * A374992  Total cost when the elements of the n-th composition (in standard
 *          order) are requested from a self-organizing list initialized to
 *          (1, 2, 3, ...), using the move-to-front updating strategy.
 * https://oeis.org/A374992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = cost of the n‑th composition (standard order) (sum
 *            increasing, within each sum: reverse lexicographic order on the
 *            parts)
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 85 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t _cost_of_composition(const idx_t parts[], idx_t len)
{
    idx_t sum = 0;
    for (idx_t i = 0; i < len; ++i) sum += parts[i];

    /* the list is initially (1,2,3,…,sum) */
    idx_t list[30];                     /* 30 > maximum possible sum (≤14) */
    for (idx_t i = 0; i < sum; ++i) list[i] = i + 1;

    seq_t cost = 0;
    for (idx_t i = 0; i < len; ++i) {
        idx_t val = parts[i];

        /* find position of val (0‑based) */
        idx_t pos = 0;
        while (list[pos] != val) ++pos;

        cost += (seq_t)(pos + 1);      /* 1‑based position is the cost */

        /* move-to-front */
        if (pos > 0) {
            idx_t tmp = list[pos];
            for (idx_t j = pos; j > 0; --j) list[j] = list[j - 1];
            list[0] = tmp;
        }
    }
    return cost;
}

/* Generate all compositions of 'rem' with the first part in descending
   order, recursing for the remainder.  When the target composition
   number is reached, store its cost in *output. */
static bool _generate_compositions_rev(idx_t rem,
                                       idx_t depth,
                                       idx_t *parts,
                                       idx_t target,
                                       idx_t *counter,
                                       seq_t *output)
{
    if (rem == 0) {
        (*counter)++;
        if (*counter == target) {
            *output = _cost_of_composition(parts, depth);
            return true;
        }
        return false;
    }

    for (idx_t part = rem; part > 0; --part) {
        parts[depth] = part;
        if (_generate_compositions_rev(rem - part, depth + 1,
                                       parts, target, counter, output))
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* empty composition */

    idx_t parts[32];                 /* buffer for a composition */
    idx_t counter = 0;
    seq_t result = 0;

    for (idx_t sum = 1; ; ++sum) {
        if (_generate_compositions_rev(sum, 0, parts,
                                      n, &counter, &result))
            return result;
    }
}
