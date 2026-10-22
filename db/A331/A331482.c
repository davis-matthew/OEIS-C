/*
 * A331482  Position of the first letter of n (in Dutch) in the alphabet.
 * https://oeis.org/A331482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* First letter position in Dutch name of n. */
#include <stdint.h>
#include <stddef.h>

/* Unit mapping: 0..19. */
static const char _unit_first[20] = {
    'n', /* 0 "nul" */
    'e', /* 1 "een" */
    't', /* 2 "twee" */
    'd', /* 3 "drie" */
    'v', /* 4 "vier" */
    'v', /* 5 "vijf" */
    'z', /* 6 "zes" */
    'z', /* 7 "zeven" */
    'a', /* 8 "acht" */
    'n', /* 9 "negen" */
    't', /*10 "tien" */
    'e', /*11 "elf" */
    't', /*12 "twaalf" */
    'd', /*13 "dertien" */
    'v', /*14 "veertien" */
    'v', /*15 "vijftien" */
    'z', /*16 "zestien" */
    'z', /*17 "zeventien" */
    'a', /*18 "achttien" */
    'n'  /*19 "negentien" */
};

/* Tens mapping: 20,30,...,90. */
static const char _tens_first[10] = {
    0,0,          /* 0-9 unused */
    't',          /*20 "twintig"  */
    'd',          /*30 "dertig"   */
    'v',          /*40 "veertig"  */
    'v',          /*50 "vijftig"  */
    'z',          /*60 "zestig"   */
    'z',          /*70 "zeventig" */
    't',          /*80 "tachtig"  */
    'n'           /*90 "negentig" */
};

/* Scale values and their first letter. */
struct _scale_t {
    uint64_t val;
    char first;
};
static const struct _scale_t _scales[] = {
    {1000000000000000000ULL, 't'}, /* trilljoen      */
    {1000000000000000ULL,    'b'}, /* biljard        */
    {1000000000000ULL,       'b'}, /* biljoen        */
    {1000000000ULL,          'm'}, /* miljard        */
    {1000000ULL,             'm'}, /* miljoen        */
    {1000ULL,                'd'}  /* duizend        */
};

/* Return the first letter of the Dutch word for n. */
static char first_letter_of_uint64(uint64_t n) {
    if (n < 20) {
        return _unit_first[(int)n];
    }
    if (n < 100) {
        int r = (int)(n % 10);
        if (r == 0) {
            return _tens_first[(int)(n / 10)];
        }
        return _unit_first[r];
    }
    if (n < 1000) {
        uint64_t q = n / 100;
        if (q == 1) {
            return 'h'; /* honderd */
        }
        return first_letter_of_uint64(q);
    }
    /* Handle larger scales. */
    for (size_t i = 0; i < sizeof(_scales) / sizeof(_scales[0]); ++i) {
        if (n >= _scales[i].val) {
            uint64_t q = n / _scales[i].val;
            if (q == 1) {
                return _scales[i].first;
            }
            return first_letter_of_uint64(q);
        }
    }
    /* Should never reach here. */
    return 'a';
}

seq_t a(idx_t n) {
    char c = first_letter_of_uint64((uint64_t)n);
    return (seq_t)(c - 'a' + 1);
}
