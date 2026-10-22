/*
 * A347806  Expansion of (theta_3(x) - 1)^3 / (4 * (3 - theta_3(x))).
 * https://oeis.org/A347806
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n into 3 or more squares
 *            (ordered partitions). For n ≥ 3 a(n) = f[n] – [n is a square] –
 *            pairs[n] where f[n] counts all compositions into squares
 *            (including 0‐to‑1 parts) and pairs[n] counts ordered pairs of
 *            squares summing to n
 * Range:     u8 n <= 18, u16 n <= 33, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *f = NULL;          /* total compositions of i into squares */
static uint8_t *is_sq = NULL;    /* is i a perfect square? */
static idx_t *pairs = NULL;      /* number of ordered square pairs summing to i */
static idx_t *sqs = NULL;        /* list of square values ≤ current capacity */
static size_t sqs_len = 0;       /* number of stored squares */
static idx_t cap = 0;            /* current capacity (≥ needed index + 1) */

/* Allocate or grow the data structures so that indices up to n exist */
static void ensure(idx_t n)
{
    if (n + 1 <= cap) return;           /* already available */

    size_t new_cap = (size_t)(n + 1);

    /* Re‑allocate arrays – cast to avoid C++ void* conversion error */
    f = (seq_t *)realloc(f, new_cap * sizeof(seq_t));
    is_sq = (uint8_t *)realloc(is_sq, new_cap * sizeof(uint8_t));
    pairs = (idx_t *)realloc(pairs, new_cap * sizeof(idx_t));

    /* Initialize newly added entries */
    if (cap == 0) {
        for (size_t i = 0; i < new_cap; ++i) {
            f[i] = 0;
            is_sq[i] = 0;
            pairs[i] = 0;
        }
    } else {
        for (size_t i = cap; i < new_cap; ++i) {
            f[i] = 0;
            is_sq[i] = 0;
            pairs[i] = 0;
        }
    }

    /* Extend the list of square numbers up to the new capacity */
    for (idx_t k = sqs_len + 1; ; ++k) {
        idx_t sq = k * k;
        if ((idx_t)sq >= new_cap) break;
        /* realloc the square list for one more element */
        sqs = (idx_t *)realloc(sqs, (sqs_len + 1) * sizeof(idx_t));
        sqs[sqs_len++] = sq;
        is_sq[sq] = 1;                   /* mark as a square */
    }

    /* Dynamic programming: compute f[i] and pairs[i] for new indices */
    if (cap == 0) {
        f[0] = 1;            /* empty composition */
        cap = 1;
    }

    for (idx_t i = cap; i <= n; ++i) {
        seq_t sum = 0;
        for (size_t j = 0; j < sqs_len && sqs[j] <= i; ++j) {
            sum += f[i - sqs[j]];
        }
        f[i] = sum;

        /* Count ordered square pairs adding to i */
        idx_t cnt = 0;
        for (size_t j = 0; j < sqs_len && sqs[j] <= i; ++j) {
            if (is_sq[i - sqs[j]]) ++cnt;
        }
        pairs[i] = cnt;
    }
    cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 3) return 0;         /* undefined below offset – return 0 */
    ensure(n);
    seq_t ans = f[n];
    if (is_sq[n]) ans -= 1;       /* exclude one‑part composition */
    ans -= (seq_t)pairs[n];       /* exclude  two‑part compositions */
    return ans;
}
