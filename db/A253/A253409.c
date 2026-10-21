/*
 * A253409  The number of ways to write an n-bit binary string and then define
 *          each run of ones as an element in an equivalence relation, and
 *          each run of zeros as an element in a second equivalence relation.
 * https://oeis.org/A253409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * Sum_{k=1..ceil(n/2)} (C(n-1, 2k-1)*Bell(k)^2 +
 *            C(n-1,2k-2)*Bell(k)*Bell(k-1))
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 17, u64 n <= 28, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* ---- auxiliary: binomial coefficient ---------------------------------- */
static inline seq_t my_binomial(idx_t n, idx_t k) {
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    idx_t i;
    for (i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res = res / i;
    }
    return res;
}

/* ---- store Bell numbers ------------------------------------------------ */
static seq_t *bell = NULL;
static size_t bell_cap = 0;
static size_t bell_len = 0;

static void ensure_bell(size_t need) {
    if (need < bell_len) return;
    if (bell_cap <= need) {
        size_t new_cap = bell_cap ? bell_cap << 1 : 1;
        while (new_cap <= need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(bell, new_cap * sizeof(seq_t));
        bell = tmp;
        bell_cap = new_cap;
    }
    for (size_t m = bell_len; m <= need; ++m) {
        if (m == 0) {
            bell[0] = (seq_t)1;
            continue;
        }
        seq_t sum = (seq_t)0;
        for (size_t k = 0; k < m; ++k) {
            seq_t comb = my_binomial(m - 1, k);
            seq_t term = comb * bell[k];
            sum += term;
        }
        bell[m] = sum;
    }
    bell_len = need + 1;
}

/* ---- main sequence function ------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    idx_t kmax = (n + 1) / 2;        /* ceil(n/2) */
    ensure_bell((size_t)kmax);

    seq_t result = (seq_t)0;

    for (idx_t k = 1; k <= kmax; ++k) {
        idx_t bin1_idx = 2 * k - 1;
        idx_t bin2_idx = 2 * k - 2;

        seq_t bin1 = my_binomial(n - 1, bin1_idx);
        seq_t bin2 = my_binomial(n - 1, bin2_idx);

        seq_t bk   = bell[(size_t)k];
        seq_t bkm1 = bell[(size_t)(k - 1)];

        /* compute (bin1 * bk^2 + bin2 * bk * bkm1) * 2 */
        seq_t term1 = bin1;
        term1 = term1 * bk;
        term1 = term1 * bk;

        seq_t term2 = bin2;
        term2 = term2 * bk;
        term2 = term2 * bkm1;

        seq_t sum = term1 + term2;
        sum = sum * 2;

        result += sum;
    }

    return result;
}
