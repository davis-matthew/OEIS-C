/*
 * A296089  Filter combining the sum of divisors (A000203) and 2-adic
 *          valuation of n (A007814); restricted growth sequence transform of
 *          A286460.
 * https://oeis.org/A296089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 167, u16 n <= 9994, u32 n <= 9994, u64 n <= 9994,
 *            u128 n <= 9994, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A296089 – restricted‑growth transform of (σ(n), v₂(n))
 *  a(n) = first label of distinct pair (sum of divisors of n,
 *        2‑adic valuation of n).  */

#include <stdint.h>
#include <stdlib.h>

static uint64_t isqrt_u64(uint64_t n) {
    uint64_t low = 0, high = (n >> 1) + 1;
    if (high > n) high = n;
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        if (mid > 0 && mid > n / mid) high = mid - 1;
        else low = mid;
    }
    return low;
}

static uint64_t sigma_uint64(uint64_t n) {
    if (n == 1) return 1;
    uint64_t sum = 1;
    uint64_t r = isqrt_u64(n);
    for (uint64_t d = 2; d <= r; ++d)
        if (n % d == 0) {
            sum += d;
            uint64_t q = n / d;
            if (q != d) sum += q;
        }
    sum += n;  /* add n itself */
    return sum;
}

static unsigned v2_uint64(uint64_t n) {
    unsigned v = 0;
    while ((n & 1ULL) == 0ULL) {
        n >>= 1;
        ++v;
    }
    return v;
}

struct Node {
    seq_t sigma;      /* σ(n) */
    unsigned v2;      /* v₂(n) */
    idx_t label;      /* assignment order */
};

static seq_t *seq_arr   = NULL;     /* array of a(n) values (0‑based) */
static size_t seq_cap   = 0;
static size_t seq_len   = 0;         /* number of elements stored */

static struct Node *nodes     = NULL;
static size_t nodes_cap = 0;
static size_t nodes_len = 0;
static idx_t next_label = 1;        /* first unused label */

static void ensure_upto(idx_t n) {
    if (n <= seq_len) return;           /* already computed */

    /* enlarge seq_arr if necessary */
    if (n > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 256;
        while (new_cap < n) new_cap <<= 1;
        seq_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }

    /* compute new elements */
    for (idx_t idx = (idx_t)seq_len; idx < n; ++idx) {
        uint64_t num = (uint64_t)(idx + 1);       /* actual n */
        uint64_t sigma0 = sigma_uint64(num);
        unsigned v2 = v2_uint64(num);
        seq_t sigma = (seq_t)sigma0;              /* cast to seq_t */

        /* search for existing pair */
        idx_t label = 0;
        for (size_t j = 0; j < nodes_len; ++j)
            if (nodes[j].sigma == sigma && nodes[j].v2 == v2) {
                label = nodes[j].label;
                break;
            }

        /* if not found, create a new node */
        if (label == 0) {
            if (nodes_len >= nodes_cap) {
                size_t new_ncap = nodes_cap ? nodes_cap << 1 : 256;
                nodes = (struct Node *)realloc(nodes, new_ncap * sizeof(struct Node));
                nodes_cap = new_ncap;
            }
            label = next_label++;
            nodes[nodes_len].sigma = sigma;
            nodes[nodes_len].v2    = v2;
            nodes[nodes_len].label = label;
            ++nodes_len;
        }

        seq_arr[idx] = (seq_t)label;
    }

    seq_len = (size_t)n;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* behavior below offset irrelevant */
    ensure_upto(n);
    return seq_arr[n - 1];
}
