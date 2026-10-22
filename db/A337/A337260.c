/*
 * A337260  Compositions, sorted by increasing sum, decreasing length and
 *          increasing colexicographical order.
 * https://oeis.org/A337260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence a(n): the n-th part in the flattened list of all
 * compositions of positive integers, sorted first by increasing sum,
 * then by decreasing length, and finally by increasing colexicographic order.
 */

#include <stdint.h>
#include <stddef.h>

/* Helper: binomial coefficient C(n, k) for small n (fits in 64 bits). */
static inline idx_t binomial_u64(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    idx_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;
    }
    return res;
}

/* Helper: recursively build the composition of sum S into length L
 * with rank r (0‑based) in colexicographic order.  The parts are
 * stored in the array `parts[0..L-1]`. */
static void build_comp(idx_t S, idx_t L, idx_t r, seq_t *parts)
{
    if (L == 1) {
        parts[0] = (seq_t)S;
        return;
    }

    /* Determine aL (rightmost part) */
    for (idx_t t = 1; t <= S - L + 1; ++t) {
        idx_t count;
        if (L - 1 == 0) {
            count = 1;
        } else {
            /* Number of compositions of sum S-t into L-1 positive parts */
            count = binomial_u64(S - t - 1, L - 2);
        }
        if (r < count) {
            parts[L - 1] = (seq_t)t;
            build_comp(S - t, L - 1, r, parts);
            return;
        } else {
            r -= count;
        }
    }
}

/* Main function requested by the specification. */
seq_t a(idx_t n)
{
    /* Find the sum block that contains the n-th part. */
    idx_t remaining = n;
    idx_t sum = 0;
    while (1) {
        ++sum;
        idx_t partCount;
        if (sum == 1) {
            partCount = 1;
        } else {
            /* P(sum) = (sum + 1) * 2^(sum - 2) for sum >= 2 */
            partCount = (sum + 1) * (uint64_t)1ULL << (sum - 2);
        }
        if (remaining > partCount) {
            remaining -= partCount;
        } else {
            break;       /* sum block found */
        }
    }

    /* remaining now holds the 1‑based offset within the sum block. */
    idx_t offset = remaining;

    /* Locate the specific length block and the composition within it. */
    for (idx_t L = sum; L >= 1; --L) {
        idx_t numComps = binomial_u64(sum - 1, L - 1);
        idx_t totalParts = numComps * L;
        if (offset > totalParts) {
            offset -= totalParts;
        } else {
            /* Target composition is in this length block. */
            idx_t compIndex = (offset - 1) / L;     /* 0‑based */
            idx_t partPos   = (offset - 1) % L;     /* 0‑based */
            seq_t parts[64];                      /* enough for all sums encountered */
            build_comp(sum, L, compIndex, parts);
            return parts[partPos];
        }
    }

    /* This line should never be reached for valid input. */
    return (seq_t)0;
}
