/*
 * A295004  The number of seconds after midnight (3600*H + 60*MM + SS)
 *          corresponding to prime time numbers A295014, i.e., numbers of the
 *          form HMMSS with primes H < 24 and MM, SS < 60.
 * https://oeis.org/A295004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = 3600*H + 60*MM + SS where H, MM, SS are prime numbers (H
 *            < 24 for hours, MM < 60, SS < 60), enumerated in lexicographic
 *            order by hour, minute, second. The sequence stops after all 2601
 *            such combinations (9 hours × 17 minutes × 17 seconds). For n
 *            larger than the number of terms, the functio...
 * Range:     u8 none, u16 n <= 2023, u32 n <= 2601, u64 n <= 2601,
 *            u128 n <= 2601, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static const idx_t primes_hour[] = {2,3,5,7,11,13,17,19,23};
static const idx_t primes_min_sec[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59};
#define HOURS_COUNT 9
#define MINSEC_COUNT 17
#define TOTAL_TERMS (HOURS_COUNT * MINSEC_COUNT * MINSEC_COUNT)

seq_t a(idx_t n)
{
    if (n < 1 || n > (idx_t)TOTAL_TERMS)
        return (seq_t)0;

    /* Convert n to 0-based index */
    idx_t idx = (idx_t)(n - 1);

    idx_t hour_idx   = idx / (MINSEC_COUNT * MINSEC_COUNT);
    idx_t rem        = idx % (MINSEC_COUNT * MINSEC_COUNT);
    idx_t minute_idx = rem / MINSEC_COUNT;
    idx_t second_idx = rem % MINSEC_COUNT;

    seq_t H  = (seq_t)primes_hour[hour_idx];
    seq_t M  = (seq_t)primes_min_sec[minute_idx];
    seq_t S  = (seq_t)primes_min_sec[second_idx];

    return H * (seq_t)3600 + M * (seq_t)60 + S;
}
