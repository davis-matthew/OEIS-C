/*
 * A318269  a(n) is the number of configurations of n indistinguishable pairs
 *          placed on the vertices of the ladder graph P_2 X P_n such that all
 *          but 4 such pairs are joined by an edge.
 * https://oeis.org/A318269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A318269: number of "(n-4)-domino" configurations on a 2×n ladder.
   Coefficients are computed from the rational generating function
   G(x) = x^2*(5x^10 + 10x^9 + 93x^8 + 230x^7 + 502x^6 + 612x^5 + 447x^4 + 158x^3 + 21x^2)
/            ((1 - x)^4 * (1 - x - x^2)^5).
   The denominator polynomial D(x) = Σ_{k=0}^{14} d_k x^k equals
   1 - 9x + 31x² - 44x³ - 4x⁴ + 84x⁵ - 66x⁶ - 46x⁷ + 74x⁸ + 4x⁹ - 36x¹⁰ + 4x¹¹ + 9x¹² - x¹³ - x¹⁴.
   The sequence satisfies the recurrence
        a(n) = p(n) – ∑_{k=1}^{14} d_k a(n‑k)       (n ≥ 0)
   with p(n) the coefficient of xⁿ in the numerator (p(4)=21, p(5)=158, …, p(12)=5, all other p(k)=0). */

#include <stdlib.h>   /* for malloc, realloc, abort */

static const int d_coeff[15] = {
    1,   -9,   31,  -44,   -4,  84, -66, -46,  74,   4, -36,   4,   9, -1, -1
};

static const seq_t p_coeff[13] = {
    0, 0, 0, 0, 21, 158, 447, 612, 502, 230, 93, 10, 5
};

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t cache_cap  = 0;

static void ensure_capacity(idx_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = (size_t)need;
    if (new_cap == 0) new_cap = 1;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) abort();
    cache = new_ptr;
    cache_cap = new_cap;
}

static void compute_up_to(idx_t n)
{
    if (n < cache_size) return;
    ensure_capacity(n + 1);

    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t sum = (i <= 12 ? p_coeff[i] : (seq_t)0);

        for (int k = 1; k <= 14 && (idx_t)k <= i; ++k) {
            int dk = d_coeff[k];
            if (dk == 0) continue;
            seq_t prod = cache[i - k] * (seq_t)(dk >= 0 ? dk : -dk);
            if (dk >= 0)
                sum -= prod;
            else
                sum += prod;
        }
        cache[i] = sum;
    }
    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    compute_up_to(n);
    return cache[n];
}
