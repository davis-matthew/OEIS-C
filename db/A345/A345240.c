/*
 * A345240  a(1) = 1; thereafter a(n) = A161406(a(n-1)).
 * https://oeis.org/A345240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, easy
 * Formula:   a(1) = 1; a(n) = A161406(a(n-1)) for n > 1, where A161406(m) is
 *            the sum of letter values (a=1,...,ñ=15,...,z=27) of the Spanish
 *            name of m
 * Range:     u8 n <= 4, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t A161406_sum_cero = 43;
static const seq_t A161406_unit_sum[10] = {0, 52, 40, 65, 82, 45, 54, 60, 43, 69};
static const seq_t A161406_tens_sum[10] = {0,0,0,90,86,92,86,87,68,94};

static const seq_t A161406_diez = 45;
static const seq_t A161406_once = 38;
static const seq_t A161406_doce = 28;
static const seq_t A161406_trece = 53;
static const seq_t A161406_catorce = 68;
static const seq_t A161406_quince = 71;
static const seq_t A161406_dieciseis = 84;
static const seq_t A161406_diecisiete = 90;
static const seq_t A161406_dieciocho = 73;
static const seq_t A161406_diecinueve = 99;
static const seq_t A161406_veinte = 77;
static const seq_t A161406_veinti = 81;
static const seq_t A161406_cien = 31;
static const seq_t A161406_ciento = 68;
static const seq_t A161406_cientos = 88;
static const seq_t A161406_mil = 34;
static const seq_t A161406_millon = 76;
static const seq_t A161406_millones = 101;
static const seq_t A161406_y = 26;
static const seq_t A161406_un_million = 36;

static seq_t A161406_two_digits(uint64_t m)
{
    if (m < 10)
        return A161406_unit_sum[(int)m];
    switch ((int)m) {
        case 10: return A161406_diez;
        case 11: return A161406_once;
        case 12: return A161406_doce;
        case 13: return A161406_trece;
        case 14: return A161406_catorce;
        case 15: return A161406_quince;
        case 16: return A161406_dieciseis;
        case 17: return A161406_diecisiete;
        case 18: return A161406_dieciocho;
        case 19: return A161406_diecinueve;
        case 20: return A161406_veinte;
        default: break;
    }
    if (m <= 29)
        return A161406_veinti + A161406_unit_sum[(int)(m - 20)];
    uint64_t tens = m / 10;
    uint64_t unit = m % 10;
    seq_t sum = A161406_tens_sum[(int)tens];
    if (unit != 0)
        sum += A161406_y + A161406_unit_sum[(int)unit];
    return sum;
}

static seq_t A161406_small(uint64_t m)
{
    seq_t sum = 0;
    uint64_t h = m / 100;
    uint64_t rem = m % 100;
    if (h > 0) {
        if (h == 1) {
            if (rem == 0)
                return A161406_cien;
            sum += A161406_ciento;
        } else {
            sum += A161406_unit_sum[(int)h] + A161406_cientos;
        }
    }
    if (rem > 0)
        sum += A161406_two_digits(rem);
    return sum;
}

static seq_t A161406_word_sum(uint64_t n)
{
    seq_t total = 0;
    uint64_t millions = n / 1000000ULL;
    n = n % 1000000ULL;
    if (millions > 0) {
        if (millions == 1)
            total += A161406_un_million + A161406_millon;
        else
            total += A161406_word_sum(millions) + A161406_millones;
    }
    uint64_t thousands = n / 1000ULL;
    n = n % 1000ULL;
    if (thousands > 0) {
        if (thousands == 1)
            total += A161406_mil;
        else
            total += A161406_small(thousands) + A161406_mil;
    }
    if (n > 0)
        total += A161406_small(n);
    return total;
}

static seq_t A161406_a(uint64_t n)
{
    if (n == 0)
        return A161406_sum_cero;
    return A161406_word_sum(n);
}

seq_t a(idx_t n)
{
    /* Periodic with period 5 from n = 10 on. */
    if (n > 10)
        n = 10 + (n - 10) % 5;
    seq_t v = 1;
    for (idx_t i = 1; i < n; ++i)
        v = A161406_a((uint64_t)v);
    return v;
}
