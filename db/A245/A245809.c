/*
 * A245809  Divisors of 4620.
 * https://oeis.org/A245809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A245809: list of all divisors of 4620 in ascending order */

#include <stddef.h>

static seq_t divs[48];
static size_t divs_count = 0;

/* Recursive generation of divisors of 4620 */
static void gen_divs(int idx, seq_t current)
{
    static const uint8_t primes[] = {2, 3, 5, 7, 11};
    static const int exps[]   = {2, 1, 1, 1, 1};
    const size_t prime_cnt = sizeof(primes)/sizeof(primes[0]);

    if (idx == (int)prime_cnt) {
        divs[divs_count++] = current;
        return;
    }
    seq_t prod = current;
    for (int e = 0; e <= exps[idx]; ++e) {
        gen_divs(idx + 1, prod);
        prod *= (seq_t)primes[idx];
    }
}

/* Simple insertion sort for a small number of elements */
static void sort_divs(void)
{
    for (size_t i = 1; i < divs_count; ++i) {
        seq_t key = divs[i];
        long j = (long)i - 1;
        while (j >= 0 && divs[j] > key) {
            divs[j + 1] = divs[j];
            --j;
        }
        divs[j + 1] = key;
    }
}

/* Initialize the divisor table on first use */
static void init_divisors(void)
{
    if (divs_count != 0)
        return;
    gen_divs(0, (seq_t)1);
    sort_divs();
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n=0, return 0 */
    init_divisors();
    if (n <= divs_count)
        return divs[n - 1];
    return (seq_t)0;   /* out of range */
}
