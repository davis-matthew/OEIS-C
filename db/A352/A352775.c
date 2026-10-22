/*
 * A352775  a(n) = pi(n) * (Sum_{n <= q < 2n, q prime} q) + (pi(2n-1) -
 *          pi(n-1)) * (Sum_{p <= n, p prime} p).
 * https://oeis.org/A352775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi(n) * Sum_{n <= q < 2n, q prime} q + (pi(2n-1)-pi(n-1))
 *            * Sum_{p <= n, p prime} p
 * Range:     u8 n <= 9, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/* Sieve data cached between calls. */
static size_t _limit = 0;
static unsigned char *_is_prime = NULL;
static idx_t *_pi_count = NULL;
static seq_t *_sum_prime = NULL;

/* Ensure that sieve tables are ready up to `need` (inclusive). */
static void _ensure_limit(idx_t need)
{
    if ((idx_t)_limit >= need) return;

    /* New size must accommodate indices 0..need. */
    size_t new_limit = (size_t)need + 1;

    /* Reallocate arrays.  Cast the void* returned by realloc for C++. */
    unsigned char *tmp_is = (unsigned char*)realloc(_is_prime,
                                 new_limit * sizeof(unsigned char));
    if (!tmp_is) exit(1);
    _is_prime = tmp_is;

    idx_t *tmp_pi   = (idx_t*)realloc(_pi_count,
                        new_limit * sizeof(idx_t));
    if (!tmp_pi) exit(1);
    _pi_count = tmp_pi;

    seq_t *tmp_sum  = (seq_t*)realloc(_sum_prime,
                        new_limit * sizeof(seq_t));
    if (!tmp_sum) exit(1);
    _sum_prime = tmp_sum;

    /* Initialise sieving flags. */
    _is_prime[0] = 0;
    if (_limit + 1 > 1) _is_prime[1] = 0;
    for (size_t i = 2; i < new_limit; ++i) _is_prime[i] = 1;

    /* Simple Eratosthenes sieve. */
    size_t sq = (size_t) sqrt((double)(new_limit - 1));
    for (size_t i = 2; i <= sq; ++i) {
        if (_is_prime[i]) {
            for (size_t j = i * i; j < new_limit; j += i)
                _is_prime[j] = 0;
        }
    }

    /* Build cumulative counts and sums. */
    _pi_count[0] = 0;
    _sum_prime[0] = 0;
    idx_t pc = 0;
    seq_t ps = 0;
    for (size_t i = 1; i < new_limit; ++i) {
        if (_is_prime[i]) {
            ++pc;
            ps += (seq_t)i;
        }
        _pi_count[i]  = pc;
        _sum_prime[i] = ps;
    }
    _limit = new_limit - 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* Outside the defined offset */

    idx_t need = 2 * n - 1;
    _ensure_limit(need);

    idx_t pi_n   = _pi_count[n];
    idx_t pi_2n1 = _pi_count[(size_t)need];
    idx_t pi_n1  = _pi_count[n - 1];

    seq_t sum_2n1 = _sum_prime[(size_t)need];
    seq_t sum_n1  = _sum_prime[(size_t)(n - 1)];
    seq_t sum_n   = _sum_prime[(size_t)n];

    seq_t part1 = (seq_t)pi_n * (sum_2n1 - sum_n1);
    seq_t part2 = ((seq_t)(pi_2n1 - pi_n1)) * sum_n;
    return part1 + part2;
}
