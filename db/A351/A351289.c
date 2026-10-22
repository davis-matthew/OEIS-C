/*
 * A351289  Square array read by descending antidiagonals: A(n,k) is the
 *          smallest m such that the base-n expansion of m contains the base-n
 *          expansions of the k-th row of A048793 as substrings.
 * https://oeis.org/A351289
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 176, u16 n <= 1005, u32 n <= 5051, u64 n <= 5051,
 *            u128 n <= 5051, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A351289, a(n), by finding the shortest superstring of
   base‑n digit strings corresponding to the set bits of the column k in
   the descending‑antidiagonal array.  The function is purely
   deterministic, uses only integer arithmetic, and is fully
   compliant with the restrictions above. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Convert a non‑negative integer num into its base‑base representation
   in most‑significant‑first order.  The caller must free the returned
   buffer.  The buffer is allocated with malloc. */
static void digits_of_uint(uint64_t num, uint64_t base,
                           unsigned char **bufp, size_t *lenp)
{
    unsigned char tmp[128];
    size_t pos = 0;
    if (num == 0) {
        tmp[pos++] = 0;
    } else {
        while (num) {
            tmp[pos++] = (unsigned char)(num % base);
            num /= base;
        }
    }
    *lenp = pos;
    unsigned char *buf = (unsigned char *)malloc(pos);
    if (!buf) abort();
    for (size_t i = 0; i < pos; ++i)
        buf[i] = tmp[pos - 1 - i];
    *bufp = buf;
}

/* Count 1‑bits of a 64‑bit integer. */
static unsigned int popcount_uint64(uint64_t x)
{
    return (unsigned int)__builtin_popcountll(x);
}

/* Compute A(row, col) – the least base‑row integer whose representation
   contains each active bit of col (treated as the position i in 1‑based
   numbering) as a substring of its base‑row digits. */
static seq_t compute_minimal_m(uint64_t n, uint64_t k)
{
    unsigned int bits = popcount_uint64(k);
    if (bits == 0) return 1;          /* k = 0 never occurs for col ≥ 1 */

    /* Allocate arrays for the substrings that must appear. */
    unsigned char **sub = (unsigned char **)malloc(bits * sizeof(unsigned char *));
    size_t *sublen = (size_t *)malloc(bits * sizeof(size_t));
    if (!sub || !sublen) abort();

    unsigned int idx = 0;
    for (uint64_t bit = 0; bit < 64 && (k >> bit); ++bit) {
        if (k & (1ULL << bit)) {
            digits_of_uint(bit + 1, n, &sub[idx], &sublen[idx]);
            ++idx;
        }
    }

    /* Pre‑compute pairwise overlaps of the substrings. */
    unsigned int **ov = (unsigned int **)malloc(bits * sizeof(unsigned int *));
    if (!ov) abort();
    for (unsigned int i = 0; i < bits; ++i) {
        ov[i] = (unsigned int *)malloc(bits * sizeof(unsigned int));
        if (!ov[i]) abort();
    }

    for (unsigned int i = 0; i < bits; ++i) {
        for (unsigned int j = 0; j < bits; ++j) {
            if (i == j) { ov[i][j] = 0; continue; }
            unsigned int max = sublen[i] < sublen[j] ? sublen[i] : sublen[j];
            unsigned int best = 0;
            for (unsigned int l = max; l > 0; --l) {
                if (memcmp(sub[i] + sublen[i] - l, sub[j], l) == 0) {
                    best = l;
                    break;
                }
            }
            ov[i][j] = best;
        }
    }

    unsigned int total = 1U << bits;
    /* dp[mask * bits + last] stores the best (shortest & lexicographically
       smallest) concatenation that uses the subset mask and ends with
       substring ‘last’.  The array is allocated lazily; uninitialized
       entries are NULL. */
    unsigned char **dp = (unsigned char **)calloc(total * bits,
                                                   sizeof(unsigned char *));
    size_t *dp_len = (size_t *)calloc(total * bits, sizeof(size_t));
    if (!dp || !dp_len) abort();

    /* Initialise single‑bit DP states. */
    for (unsigned int i = 0; i < bits; ++i) {
        unsigned int mask = 1U << i;
        dp[mask * bits + i] = (unsigned char *)malloc(sublen[i]);
        memcpy(dp[mask * bits + i], sub[i], sublen[i]);
        dp_len[mask * bits + i] = sublen[i];
    }

    /* DP transition: try to append each remaining substring to the best
       existing state. */
    for (unsigned int mask = 1; mask < total; ++mask) {
        for (unsigned int last = 0; last < bits; ++last) {
            unsigned char *cur = dp[mask * bits + last];
            if (!cur) continue;
            size_t curlen = dp_len[mask * bits + last];
            for (unsigned int nxt = 0; nxt < bits; ++nxt) {
                if (mask & (1U << nxt)) continue;
                unsigned int new_mask = mask | (1U << nxt);
                unsigned int add = sublen[nxt] - ov[last][nxt];
                unsigned int newlen = (unsigned int)(curlen + add);
                unsigned char *cand = (unsigned char *)malloc(newlen);
                if (!cand) abort();
                memcpy(cand, cur, curlen);
                memcpy(cand + curlen, sub[nxt] + ov[last][nxt], add);

                unsigned char *exist = dp[new_mask * bits + nxt];
                size_t existlen = dp_len[new_mask * bits + nxt];
                bool better = false;
                if (!exist) better = true;
                else if (newlen < existlen) better = true;
                else if (newlen == existlen &&
                         memcmp(cand, exist, newlen) < 0)
                    better = true;

                if (better) {
                    if (exist) free(exist);
                    dp[new_mask * bits + nxt] = cand;
                    dp_len[new_mask * bits + nxt] = newlen;
                } else {
                    free(cand);
                }
            }
        }
    }

    /* Extract the best solution over all possible ending substrings. */
    unsigned int full_mask = total - 1;
    unsigned char *best = NULL;
    size_t bestlen = 0;
    for (unsigned int last = 0; last < bits; ++last) {
        unsigned char *cand = dp[full_mask * bits + last];
        if (!cand) continue;
        size_t candlen = dp_len[full_mask * bits + last];
        bool better = false;
        if (!best) better = true;
        else if (candlen < bestlen) better = true;
        else if (candlen == bestlen && memcmp(cand, best, candlen) < 0)
            better = true;
        if (better) {
            best = cand;
            bestlen = candlen;
        }
    }

    /* Convert the best digit array back into a seq_t numeric value. */
    seq_t val = 0;
    if (best) {
        for (size_t i = 0; i < bestlen; ++i)
            val = (seq_t)(( (seq_t)val * (seq_t)n ) + (seq_t)best[i]);
    }

    /* Free all dynamically allocated memory. */
    for (unsigned int i = 0; i < bits; ++i) free(sub[i]);
    free(sub); free(sublen);
    for (unsigned int i = 0; i < bits; ++i) free(ov[i]);
    free(ov);
    for (unsigned int m = 0; m < total * bits; ++m)
        if (dp[m]) free(dp[m]);
    free(dp); free(dp_len);

    return val;
}

/* Map the sequence index to the corresponding (row, col) in the
   descending‑antidiagonal array and compute the value.  The offset
   counting starts at n = 2. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;    /* undefined for n < 2 */

    uint64_t offset = (uint64_t)(n - 2);   /* 0‑based offset */

    /* Find diagonal s (row + col = s) that contains the element. */
    uint64_t cum = 0;
    uint64_t s = 3;
    while (cum <= offset) {
        cum += s - 2;   /* cells on this diagonal: (s-2) */
        ++s;
    }
    --s;                       /* actual diagonal */
    uint64_t cum_prev = cum - (s - 2);
    uint64_t pos_in_diag = offset - cum_prev;   /* 0‑based within diagonal */

    /* Row increases, column decreases. */
    uint64_t row = 2 + pos_in_diag;      /* row index (≥ 2) */
    uint64_t col = s - row;              /* column index (≥ 1) */

    return compute_minimal_m(row, col);
}
