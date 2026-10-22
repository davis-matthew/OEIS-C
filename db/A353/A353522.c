/*
 * A353522  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A000035(i) = A000035(j) and A003415(i) = A003415(j), for all i,
 *          j >= 1, where A000035 and A003415 compute the parity and the
 *          arithmetic derivative of their argument.
 * https://oeis.org/A353522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 188, u16 n <= 9822, u32 n <= 9822, u64 n <= 9822,
 *            u128 n <= 9822, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the lexicographically earliest label assigned to the pair
 * (parity(n), arithmetic_derivative(n)).  The sequence is a restricted
 * growth sequence over pairs of (parity, derivative) where a new pair receives
 * the smallest unused integer, and equal pairs receive the same integer.
 *
 * The function builds the sequence on demand and memoises all values up to
 * the largest requested index.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper to compute the arithmetic derivative of an integer n.
 * Derivative of n = n * sum_{p^e||n} (e / p).
 */
static seq_t derivative(idx_t n)
{
    idx_t temp = n;
    seq_t nn   = (seq_t)n;
    seq_t der  = 0;

    /* factor 2 separately, as we need integer division */
    int e = 0;
    while ((temp & 1) == 0) {
        temp >>= 1;
        ++e;
    }
    if (e > 0) {
        der += ((seq_t)e) * nn / (seq_t)2;
    }

    /* odd factors */
    for (int p = 3; (idx_t)p * (idx_t)p <= temp; p += 2) {
        if (temp % p == 0) {
            e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            der += ((seq_t)e) * nn / (seq_t)p;
        }
    }

    /* remaining prime factor, if any */
    if (temp > 1) {
        /* e == 1 */
        der += nn / (seq_t)temp;
    }

    return der;
}

/* Internal structure to store previously seen pairs. */
struct pair_info {
    uint8_t parity;    /* 0 for even, 1 for odd */
    seq_t  deriv;      /* arithmetic derivative */
    seq_t  label;      /* assigned sequence value */
};

/* Static state (memoisation) */
static seq_t *sequence       = NULL;   /* sequence values, 0‑based (a(1) at index 0) */
static size_t  seq_len       = 0;      /* number of values stored */
static size_t  seq_cap       = 0;      /* allocated capacity */

static struct pair_info *pairs   = NULL;   /* stored pairs */
static size_t  pair_len          = 0;      /* number of pairs stored */
static size_t  pair_cap          = 0;      /* allocated capacity */

/* Ensure there is room for at least 'need' elements in an array. */
static void ensure_capacity(size_t *cap, void **ptr, size_t need, size_t elem_size)
{
    if (need <= *cap) return;
    size_t new_cap = (*cap > 0) ? *cap * 2 : 8;
    if (new_cap < need) new_cap = need;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) {
        /* On failure, program would abort; but the contest environment guarantees memory. */
        abort();
    }
    *ptr = tmp;
    *cap = new_cap;
}

/* Main function implementing the sequence. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for indices < 1 */

    /* Grow sequence array if needed */
    if ((size_t)n > seq_len) {
        ensure_capacity(&seq_cap, (void **)&sequence, (size_t)n, sizeof(seq_t));
        /* Also grow pairs array as we may need to add new pairs */
        ensure_capacity(&pair_cap, (void **)&pairs, pair_len + (size_t)(n - seq_len),
                        sizeof(struct pair_info));

        /* Compute values for all indices from seq_len+1 to n */
        for (size_t idx = seq_len + 1; idx <= (size_t)n; ++idx) {
            idx_t i = (idx_t)idx;
            uint8_t parity = (i & 1) ? 1 : 0;
            seq_t deriv = derivative(i);

            /* Search for existing pair */
            seq_t label = 0;
            for (size_t j = 0; j < pair_len; ++j) {
                if (pairs[j].parity == parity && pairs[j].deriv == deriv) {
                    label = pairs[j].label;
                    break;
                }
            }

            /* Assign new label if pair not seen before */
            if (label == 0) {
                label = (seq_t)(pair_len + 1);  /* labels start at 1 */
                pairs[pair_len].parity = parity;
                pairs[pair_len].deriv  = deriv;
                pairs[pair_len].label  = label;
                ++pair_len;
            }

            sequence[idx - 1] = label;   /* store value */
        }
        seq_len = (size_t)n;
    }

    return sequence[n - 1];
}
