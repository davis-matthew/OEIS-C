/*
 * A303274  Decimal expansion of 1200/3937.
 * https://oeis.org/A303274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A303274: Decimal expansion of 1200/3937
   a(n) = the n-th decimal digit of 1200/3937 (a(0) is the first digit after the decimal point).
   The implementation uses long division with memoisation. */
#include <stdlib.h>
#if defined(__cplusplus)
#    include <vector>
#endif

seq_t a(idx_t n)
{
    /* constant denominator and initial remainder */
    const seq_t den = 3937;
    const seq_t init_rem = 1200; /* 1200 % 3937 = 1200 */

#if defined(__cplusplus)
    /* C++ implementation using std::vector for automatic growth */
    static std::vector<seq_t> digits;
    static seq_t rem = init_rem;

    /* initialise vector on first call */
    if (digits.empty())
        rem = init_rem;

    /* generate digits up to n */
    while (static_cast<idx_t>(digits.size()) <= n) {
        seq_t temp = rem * 10;
        seq_t digit = temp / den;
        rem = temp % den;
        digits.push_back(digit);
    }
    return digits[static_cast<idx_t>(n)];
#else
    /* C implementation with manual memory management */
    static seq_t *digits = NULL;
    static size_t cap = 0;
    static idx_t computed_upto = 0;
    static seq_t rem = init_rem;

    /* expand capacity if necessary */
    if (digits == NULL && cap == 0) {
        cap = 128;
        digits = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    while (computed_upto <= n) {
        if (cap <= (size_t)computed_upto) {
            size_t new_cap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(digits, new_cap * sizeof(seq_t));
            if (!tmp) {
                /* allocation failed – abort */
                abort();
            }
            digits = tmp;
            cap = new_cap;
        }

        seq_t temp = rem * 10;
        seq_t digit = temp / den;
        rem = temp % den;
        digits[computed_upto++] = digit;
    }

    return digits[ (size_t)n ];
#endif
}
