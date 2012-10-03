/*
 * ArcScripts for ArcEmu MMORPG Server
 * Copyright (C) 2009 ArcEmu Team <http://www.arcemu.org/>
 * Copyright (C) 2008-2009 Sun++ Team <http://www.sunscripting.com/>
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2007-2008 Moon++ Team <http://www.moonplusplus.info/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Setup.h"

/************************************************************************/
/*  Blackrock Depths.cpp												*/
/************************************************************************/

class AmbassadorFlamelash : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(AmbassadorFlamelash);
		SP_AI_Spell spell;
		bool m_spellcheck;

		AmbassadorFlamelash(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			spell.info = dbcSpell.LookupEntry(15573);
			spell.targettype = TARGET_ATTACKING;
			spell.instant = true;
			spell.cooldown = 10;
			spell.perctrigger = RandomFloat(20.0f);
			spell.attackstoptimer = 1000;
			m_spellcheck = true;
		}

		void OnCombatStart(Unit* mTarget)
		{
			CastTime();
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void CastTime()
		{
			spell.casttime = spell.cooldown;
		}

		void OnTargetDied(Unit* mTarget)
		{
		}

		void OnCombatStop(Unit* mTarget)
		{
			CastTime();
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			CastTime();
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				spell.casttime--;

				if(m_spellcheck)
				{
					spell.casttime = spell.cooldown;
					target = _unit->GetAIInterface()->getNextTarget();
					_unit->CastSpell(target, spell.info, spell.instant);

					if(spell.speech != "")
					{
						_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spell.speech.c_str());
						_unit->PlaySoundToSet(spell.soundid);
					}

					m_spellcheck = false;
					return;
				}

				if((val > comulativeperc && val <= (comulativeperc + spell.perctrigger)) || !spell.casttime)
				{
					_unit->setAttackTimer(spell.attackstoptimer, false);
					m_spellcheck = true;
				}
				comulativeperc += spell.perctrigger;
			}
		}

};

//Easier to keep track of everything if #define is being used.
 
//Anub'shiah
#define BOSS_ANUBSHIAH			9031
 
#define AS_BANISH						8994
#define AS_COT							15470 	//Curse of Tongues
#define AS_COW							12493 	//Curse of Weakness
#define AS_DEMON_ARMOR			13787
#define AS_ENVELOPING_WEB 	15471
#define AS_SHADOW_BOLT 			11661


class AnubShiah : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(AnubShiah);
		SP_AI_Spell spells[6];
		bool m_spellcheck[6];

		AnubShiah(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 6;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(AS_BANISH);
			spells[0].cooldown = 10;
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].perctrigger = RandomFloat(20.0f);
			spells[0].attackstoptimer = 1000;
			m_spellcheck[0] = true;


			spells[1].info = dbcSpell.LookupEntry(AS_COT);
			spells[1].cooldown = 10;
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = true;
			spells[1].perctrigger = RandomFloat(20.0f);
			spells[1].attackstoptimer = 1000;
			m_spellcheck[1] = true;
			
						 
			 spells[2].info = dbcSpell.LookupEntry(AS_COW);
		     spells[2].cooldown = 10;
		     spells[2].targettype = TARGET_ATTACKING;
		     spells[2].instant = true;
		     spells[2].perctrigger = (float)RandomFloat(20.0f);
		     spells[2].attackstoptimer = 1000;
		     m_spellcheck[2] = true;
			 
			 spells[3].info = dbcSpell.LookupEntry(AS_DEMON_ARMOR);
		     spells[3].cooldown = 10;
		     spells[3].targettype = TARGET_SELF;
		     spells[3].instant = true;
		     spells[3].perctrigger = (float)RandomFloat(20.0f);
		     spells[3].attackstoptimer = 1000;
		     m_spellcheck[3] = true;
			 
			 spells[4].info = dbcSpell.LookupEntry(AS_ENVELOPING_WEB);
		     spells[4].cooldown = 10;
		     spells[4].targettype = TARGET_ATTACKING;
		     spells[4].instant = true;
		     spells[4].perctrigger = (float)RandomFloat(20.0f);
		     spells[4].attackstoptimer = 1000;
		     m_spellcheck[4] = true;
			 
			 spells[5].info = dbcSpell.LookupEntry(AS_SHADOW_BOLT);
		     spells[5].cooldown = 10;
		     spells[5].targettype = TARGET_ATTACKING;
		     spells[5].instant = true;
		     spells[5].perctrigger = (float)RandomFloat(20.0f);
		     spells[5].attackstoptimer = 1000;
		     m_spellcheck[5] = true;

		}

		void OnCombatStart(Unit* mTarget)
		{
			CastTime();
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void CastTime()
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = spells[i].cooldown;
		}

		void OnTargetDied(Unit* mTarget)
		{
		}

		void OnCombatStop(Unit* mTarget)
		{
			CastTime();
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			CastTime();
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					spells[i].casttime--;

					if(m_spellcheck[i])
					{
						spells[i].casttime = spells[i].cooldown;
						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
						}

						if(spells[i].speech != "")
						{
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
							_unit->PlaySoundToSet(spells[i].soundid);
						}

						m_spellcheck[i] = false;
						return;
					}

					if((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						m_spellcheck[i] = true;
					}
					comulativeperc += spells[i].perctrigger;
				}
			}
		}

	protected:

		int nrspells;
};

//Bael'Gar

#define BAELGAR						9016

#define BG_SSBG						13895 // Summon Spawn of Bael'Gar
#define BG_MAGMA_SPLASH	13879 // Effect for "Summon Spawn of Bal'Gar" (?)

class BaelGar : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(BaelGar);
		SP_AI_Spell spells[2];
		bool m_spellcheck[2];

		BaelGar(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 2;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(13879);
			spells[0].cooldown = 10;
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].perctrigger = RandomFloat(20.0f);
			spells[0].attackstoptimer = 1000;
			m_spellcheck[0] = true;


			spells[1].info = dbcSpell.LookupEntry(13895);
			spells[1].cooldown = 10;
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = true;
			spells[1].perctrigger = RandomFloat(20.0f);
			spells[1].attackstoptimer = 1000;
			m_spellcheck[1] = true;

		}

		void OnCombatStart(Unit* mTarget)
		{
			CastTime();
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void CastTime()
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = spells[i].cooldown;
		}

		void OnTargetDied(Unit* mTarget)
		{
		}

		void OnCombatStop(Unit* mTarget)
		{
			CastTime();
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			CastTime();
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					spells[i].casttime--;

					if(m_spellcheck[i])
					{
						spells[i].casttime = spells[i].cooldown;
						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
						}

						if(spells[i].speech != "")
						{
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
							_unit->PlaySoundToSet(spells[i].soundid);
						}

						m_spellcheck[i] = false;
						return;
					}

					if((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						m_spellcheck[i] = true;
					}
					comulativeperc += spells[i].perctrigger;
				}
			}
		}

	protected:

		int nrspells;
};

 
/*
///
Found no documentation what so ever which proved that EDT has Cleave or Mortal Strike therefore i removed it.
///
*/
 
#define EDT 					9019 // Emperor Dagran Thaurissan

#define EDT_HOT			17492 // Hand of Thaurissan
#define EDT_AOF			15636 // Avatar of Flame


class EmperorDagranThaurissan : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(EmperorDagranThaurissan);
		SP_AI_Spell spells[2];
		bool m_spellcheck[2];

		EmperorDagranThaurissan(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 2;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(EDT_HOT);
			spells[0].cooldown = 10;
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].perctrigger = RandomFloat(20.0f);
			spells[0].attackstoptimer = 1000;
			m_spellcheck[0] = true;


			spells[1].info = dbcSpell.LookupEntry(EDT_AOF);
			spells[1].cooldown = 10;
			spells[1].targettype = TARGET_SELF;
			spells[1].instant = true;
			spells[1].perctrigger = RandomFloat(20.0f);
			spells[1].attackstoptimer = 1000;
			m_spellcheck[1] = true;

			}

		void OnCombatStart(Unit* mTarget)
		{
			CastTime();
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void CastTime()
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = spells[i].cooldown;
		}

		void OnTargetDied(Unit* mTarget)
		{
		}

		void OnCombatStop(Unit* mTarget)
		{
			CastTime();
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			CastTime();
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					spells[i].casttime--;

					if(m_spellcheck[i])
					{
						spells[i].casttime = spells[i].cooldown;
						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
						}

						if(spells[i].speech != "")
						{
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
							_unit->PlaySoundToSet(spells[i].soundid);
						}

						m_spellcheck[i] = false;
						return;
					}

					if((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						m_spellcheck[i] = true;
					}
					comulativeperc += spells[i].perctrigger;
				}
			}
		}

	protected:

		int nrspells;
};

#define EVISCERATOR 					9029
 
#define ER_VICIOUS_REND			14331
#define ER_SHADOW_BOLT			20741
#define ER_ANTI_MAGIC_SHIELD	7121


class Eviscerator : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(Eviscerator);
		SP_AI_Spell spells[3];
		bool m_spellcheck[3];

		Eviscerator(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 3;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(ER_VICIOUS_REND);
			spells[0].cooldown = 10;
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].perctrigger = RandomFloat(20.0f);
			spells[0].attackstoptimer = 1000;
			m_spellcheck[0] = true;


			spells[1].info = dbcSpell.LookupEntry(ER_SHADOW_BOLT);
			spells[1].cooldown = 10;
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = true;
			spells[1].perctrigger = RandomFloat(20.0f);
			spells[1].attackstoptimer = 1000;
			m_spellcheck[1] = true;
						 
			 spells[2].info = dbcSpell.LookupEntry(ER_ANTI_MAGIC_SHIELD);
		     spells[2].cooldown = 10;
		     spells[2].targettype = TARGET_SELF;
		     spells[2].instant = true;
		     spells[2].perctrigger = (float)RandomFloat(20.0f);
		     spells[2].attackstoptimer = 1000;
		     m_spellcheck[2] = true; 

		}

		void OnCombatStart(Unit* mTarget)
		{
			CastTime();
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void CastTime()
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = spells[i].cooldown;
		}

		void OnTargetDied(Unit* mTarget)
		{
		}

		void OnCombatStop(Unit* mTarget)
		{
			CastTime();
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			CastTime();
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					spells[i].casttime--;

					if(m_spellcheck[i])
					{
						spells[i].casttime = spells[i].cooldown;
						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
						}

						if(spells[i].speech != "")
						{
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
							_unit->PlaySoundToSet(spells[i].soundid);
						}

						m_spellcheck[i] = false;
						return;
					}

					if((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						m_spellcheck[i] = true;
					}
					comulativeperc += spells[i].perctrigger;
				}
			}
		}

	protected:

		int nrspells;
};

#define FINEOUS_DARKVIRE			9056
 
#define FD_KICK								15614
#define FD_HOLY_STRIKE					13953
#define FD_DEVOTION_AURA			8258
#define FD_HOLY_LIGHT					15943
#define FD_SEAL_OF_RECKONING	15346



class FineousDarkvire : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(FineousDarkvire);
		SP_AI_Spell spells[5];
		bool m_spellcheck[5];

		FineousDarkvire(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 5;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(FD_KICK);
			spells[0].cooldown = 10;
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].perctrigger = RandomFloat(20.0f);
			spells[0].attackstoptimer = 1000;
			m_spellcheck[0] = true;


			spells[1].info = dbcSpell.LookupEntry(FD_HOLY_STRIKE);
			spells[1].cooldown = 10;
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = true;
			spells[1].perctrigger = RandomFloat(20.0f);
			spells[1].attackstoptimer = 1000;
			m_spellcheck[1] = true;
			 
			 spells[2].info = dbcSpell.LookupEntry(FD_DEVOTION_AURA);
		     spells[2].cooldown = 10;
		     spells[2].targettype = TARGET_SELF;
		     spells[2].instant = true;
		     spells[2].perctrigger = (float)RandomFloat(20.0f);
		     spells[2].attackstoptimer = 1000;
		     m_spellcheck[2] = true;
			 
			 spells[3].info = dbcSpell.LookupEntry(FD_HOLY_LIGHT);
		     spells[3].cooldown = 10;
		     spells[3].targettype = TARGET_SELF;
		     spells[3].instant = true;
		     spells[3].perctrigger = (float)RandomFloat(20.0f);
		     spells[3].attackstoptimer = 1000;
		     m_spellcheck[3] = true;
			 
			 spells[4].info = dbcSpell.LookupEntry(FD_SEAL_OF_RECKONING);
		     spells[4].cooldown = 10;
		     spells[4].targettype = TARGET_SELF;
		     spells[4].instant = true;
		     spells[4].perctrigger = (float)RandomFloat(20.0f);
		     spells[4].attackstoptimer = 1000;
		     m_spellcheck[4] = true;
 
		}

		void OnCombatStart(Unit* mTarget)
		{
			CastTime();
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void CastTime()
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = spells[i].cooldown;
		}

		void OnTargetDied(Unit* mTarget)
		{
		}

		void OnCombatStop(Unit* mTarget)
		{
			CastTime();
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			CastTime();
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					spells[i].casttime--;

					if(m_spellcheck[i])
					{
						spells[i].casttime = spells[i].cooldown;
						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
						}

						if(spells[i].speech != "")
						{
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
							_unit->PlaySoundToSet(spells[i].soundid);
						}

						m_spellcheck[i] = false;
						return;
					}

					if((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						m_spellcheck[i] = true;
					}
					comulativeperc += spells[i].perctrigger;
				}
			}
		}

	protected:

		int nrspells;
};

#define GENERAL_ANGERFORGE		9033
#define GA_SUNDER_ARMOR			15572

class GeneralAngerforge : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(GeneralAngerforge);
		SP_AI_Spell spells[3];
		bool m_spellcheck[3];

		GeneralAngerforge(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 3;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(GA_SUNDER_ARMOR);
			spells[0].cooldown = 10;
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].perctrigger = RandomFloat(20.0f);
			spells[0].attackstoptimer = 1000;
			m_spellcheck[0] = true;
	}

		void OnCombatStart(Unit* mTarget)
		{
			CastTime();
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void CastTime()
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = spells[i].cooldown;
		}

		void OnTargetDied(Unit* mTarget)
		{
		}

		void OnCombatStop(Unit* mTarget)
		{
			CastTime();
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			CastTime();
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					spells[i].casttime--;

					if(m_spellcheck[i])
					{
						spells[i].casttime = spells[i].cooldown;
						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
						}

						if(spells[i].speech != "")
						{
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
							_unit->PlaySoundToSet(spells[i].soundid);
						}

						m_spellcheck[i] = false;
						return;
					}

					if((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						m_spellcheck[i] = true;
					}
					comulativeperc += spells[i].perctrigger;
				}
			}
		}

	protected:

		int nrspells;
};

#define GOLEM_LORD_ARGELMACH 		8983
 
#define GLA_CHAIN_LIGHTNING				15306
#define GLA_LIGHTNING_SHIELD				15507
#define GLA_SHOCK									15605

class GolemLordArgelmach : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(GolemLordArgelmach);
		SP_AI_Spell spells[3];
		bool m_spellcheck[3];

		GolemLordArgelmach(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 3;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(GLA_CHAIN_LIGHTNING);
			spells[0].cooldown = 10;
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].perctrigger = RandomFloat(20.0f);
			spells[0].attackstoptimer = 1000;
			m_spellcheck[0] = true;


			spells[1].info = dbcSpell.LookupEntry(GLA_LIGHTNING_SHIELD);
			spells[1].cooldown = 10;
			spells[1].targettype = TARGET_SELF;
			spells[1].instant = true;
			spells[1].perctrigger = RandomFloat(20.0f);
			spells[1].attackstoptimer = 1000;
			m_spellcheck[1] = true;


			spells[2].info = dbcSpell.LookupEntry(GLA_SHOCK);
			spells[2].cooldown = 10;
			spells[2].targettype = TARGET_ATTACKING;
			spells[2].instant = true;
			spells[2].perctrigger = RandomFloat(20.0f);
			spells[2].attackstoptimer = 1000;
			m_spellcheck[2] = true;

		}

		void OnCombatStart(Unit* mTarget)
		{
			CastTime();
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void CastTime()
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = spells[i].cooldown;
		}

		void OnTargetDied(Unit* mTarget)
		{
		}

		void OnCombatStop(Unit* mTarget)
		{
			CastTime();
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			CastTime();
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					spells[i].casttime--;

					if(m_spellcheck[i])
					{
						spells[i].casttime = spells[i].cooldown;
						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
						}

						if(spells[i].speech != "")
						{
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
							_unit->PlaySoundToSet(spells[i].soundid);
						}

						m_spellcheck[i] = false;
						return;
					}

					if((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						m_spellcheck[i] = true;
					}
					comulativeperc += spells[i].perctrigger;
				}
			}
		}

	protected:

		int nrspells;
};

#define GOROSH_THE_DERVISH			9027
 
#define GTD_BLOODLUST						21049
#define GTD_MORTAL_STRIKE				15708
#define GTD_WHIRLWIND						15589
#define GTD_WHIRLWIND_H					13736

class GoroshTheDervish : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(GoroshTheDervish);
		SP_AI_Spell spells[3];
		bool m_spellcheck[3];

		GoroshTheDervish(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 3;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(GTD_BLOODLUST);
			spells[0].cooldown = 10;
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].perctrigger = RandomFloat(20.0f);
			spells[0].attackstoptimer = 1000;
			m_spellcheck[0] = true;


			spells[1].info = dbcSpell.LookupEntry(GTD_MORTAL_STRIKE);
			spells[1].cooldown = 10;
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = true;
			spells[1].perctrigger = RandomFloat(20.0f);
			spells[1].attackstoptimer = 1000;
			m_spellcheck[1] = true;
			
			
			spells[2].info = dbcSpell.LookupEntry(GTD_WHIRLWIND);
			spells[2].cooldown = 10;
			spells[2].targettype = TARGET_ATTACKING;
			spells[2].instant = true;
			spells[2].perctrigger = (float)RandomFloat(20.0f);
			spells[2].attackstoptimer = 1000;
			m_spellcheck[2] = true;
 
		}

		void OnCombatStart(Unit* mTarget)
		{
			CastTime();
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void CastTime()
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = spells[i].cooldown;
		}

		void OnTargetDied(Unit* mTarget)
		{
		}

		void OnCombatStop(Unit* mTarget)
		{
			CastTime();
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			CastTime();
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					spells[i].casttime--;

					if(m_spellcheck[i])
					{
						spells[i].casttime = spells[i].cooldown;
						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
						}

						if(spells[i].speech != "")
						{
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
							_unit->PlaySoundToSet(spells[i].soundid);
						}

						m_spellcheck[i] = false;
						return;
					}

					if((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						m_spellcheck[i] = true;
					}
					comulativeperc += spells[i].perctrigger;
				}
			}
		}

	protected:

		int nrspells;
};


class Grizzle : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(Grizzle);
		SP_AI_Spell spells[2];
		bool m_spellcheck[2];

		Grizzle(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 2;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(6524);
			spells[0].cooldown = 10;
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].perctrigger = RandomFloat(20.0f);
			spells[0].attackstoptimer = 1000;
			m_spellcheck[0] = true;


			spells[1].info = dbcSpell.LookupEntry(20691);
			spells[1].cooldown = 10;
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = true;
			spells[1].perctrigger = RandomFloat(20.0f);
			spells[1].attackstoptimer = 1000;
			m_spellcheck[1] = true;

		}

		void OnCombatStart(Unit* mTarget)
		{
			CastTime();
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void CastTime()
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = spells[i].cooldown;
		}

		void OnTargetDied(Unit* mTarget)
		{
		}

		void OnCombatStop(Unit* mTarget)
		{
			CastTime();
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			CastTime();
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					spells[i].casttime--;

					if(m_spellcheck[i])
					{
						spells[i].casttime = spells[i].cooldown;
						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
						}

						if(spells[i].speech != "")
						{
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
							_unit->PlaySoundToSet(spells[i].soundid);
						}

						m_spellcheck[i] = false;
						return;
					}

					if((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						m_spellcheck[i] = true;
					}
					comulativeperc += spells[i].perctrigger;
				}
			}
		}

	protected:

		int nrspells;
};


class HedrumTheCreeper : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(HedrumTheCreeper);
		SP_AI_Spell spells[3];
		bool m_spellcheck[3];

		HedrumTheCreeper(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 3;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(15475);
			spells[0].cooldown = 10;
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].perctrigger = RandomFloat(20.0f);
			spells[0].attackstoptimer = 1000;
			m_spellcheck[0] = true;


			spells[1].info = dbcSpell.LookupEntry(15474);
			spells[1].cooldown = 10;
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = true;
			spells[1].perctrigger = RandomFloat(20.0f);
			spells[1].attackstoptimer = 1000;
			m_spellcheck[1] = true;


			spells[2].info = dbcSpell.LookupEntry(3609);
			spells[2].cooldown = 10;
			spells[2].targettype = TARGET_ATTACKING;
			spells[2].instant = true;
			spells[2].perctrigger = RandomFloat(20.0f);
			spells[2].attackstoptimer = 1000;
			m_spellcheck[2] = true;

		}

		void OnCombatStart(Unit* mTarget)
		{
			CastTime();
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void CastTime()
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = spells[i].cooldown;
		}

		void OnTargetDied(Unit* mTarget)
		{
		}

		void OnCombatStop(Unit* mTarget)
		{
			CastTime();
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			CastTime();
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					spells[i].casttime--;

					if(m_spellcheck[i])
					{
						spells[i].casttime = spells[i].cooldown;
						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
						}

						if(spells[i].speech != "")
						{
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
							_unit->PlaySoundToSet(spells[i].soundid);
						}

						m_spellcheck[i] = false;
						return;
					}

					if((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						m_spellcheck[i] = true;
					}
					comulativeperc += spells[i].perctrigger;
				}
			}
		}

	protected:

		int nrspells;
};

class HighInterrogatorGerstahn : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(HighInterrogatorGerstahn);
		SP_AI_Spell spells[3];
		bool m_spellcheck[3];

		HighInterrogatorGerstahn(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 3;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(10894);
			spells[0].cooldown = 10;
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].perctrigger = RandomFloat(20.0f);
			spells[0].attackstoptimer = 1000;
			m_spellcheck[0] = true;


			spells[1].info = dbcSpell.LookupEntry(8122);
			spells[1].cooldown = 10;
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = true;
			spells[1].perctrigger = RandomFloat(20.0f);
			spells[1].attackstoptimer = 1000;
			m_spellcheck[1] = true;


			spells[2].info = dbcSpell.LookupEntry(10876);
			spells[2].cooldown = 10;
			spells[2].targettype = TARGET_ATTACKING;
			spells[2].instant = true;
			spells[2].perctrigger = RandomFloat(20.0f);
			spells[2].attackstoptimer = 1000;
			m_spellcheck[2] = true;

		}

		void OnCombatStart(Unit* mTarget)
		{
			CastTime();
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void CastTime()
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = spells[i].cooldown;
		}

		void OnTargetDied(Unit* mTarget)
		{
		}

		void OnCombatStop(Unit* mTarget)
		{
			CastTime();
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			CastTime();
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					spells[i].casttime--;

					if(m_spellcheck[i])
					{
						spells[i].casttime = spells[i].cooldown;
						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
						}

						if(spells[i].speech != "")
						{
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
							_unit->PlaySoundToSet(spells[i].soundid);
						}

						m_spellcheck[i] = false;
						return;
					}

					if((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						m_spellcheck[i] = true;
					}
					comulativeperc += spells[i].perctrigger;
				}
			}
		}

	protected:

		int nrspells;
};

class HoundmasterGrebmar : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(HoundmasterGrebmar);
		SP_AI_Spell spells[2];
		bool m_spellcheck[2];

		HoundmasterGrebmar(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 2;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(23511);
			spells[0].cooldown = 10;
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].perctrigger = RandomFloat(20.0f);
			spells[0].attackstoptimer = 1000;
			m_spellcheck[0] = true;


			spells[1].info = dbcSpell.LookupEntry(17153);
			spells[1].cooldown = 10;
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = true;
			spells[1].perctrigger = RandomFloat(20.0f);
			spells[1].attackstoptimer = 1000;
			m_spellcheck[1] = true;

		}

		void OnCombatStart(Unit* mTarget)
		{
			CastTime();
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void CastTime()
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = spells[i].cooldown;
		}

		void OnTargetDied(Unit* mTarget)
		{
		}

		void OnCombatStop(Unit* mTarget)
		{
			CastTime();
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			CastTime();
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					spells[i].casttime--;

					if(m_spellcheck[i])
					{
						spells[i].casttime = spells[i].cooldown;
						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
						}

						if(spells[i].speech != "")
						{
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
							_unit->PlaySoundToSet(spells[i].soundid);
						}

						m_spellcheck[i] = false;
						return;
					}

					if((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						m_spellcheck[i] = true;
					}
					comulativeperc += spells[i].perctrigger;
				}
			}
		}

	protected:

		int nrspells;
};


class HurleyBlackbreath : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(HurleyBlackbreath);
		SP_AI_Spell spells[3];
		bool m_spellcheck[3];

		HurleyBlackbreath(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 3;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(17294);
			spells[0].cooldown = 10;
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].perctrigger = RandomFloat(20.0f);
			spells[0].attackstoptimer = 1000;
			m_spellcheck[0] = true;


			spells[1].info = dbcSpell.LookupEntry(15583);
			spells[1].cooldown = 10;
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = true;
			spells[1].perctrigger = RandomFloat(20.0f);
			spells[1].attackstoptimer = 1000;
			m_spellcheck[1] = true;


			spells[2].info = dbcSpell.LookupEntry(14099);
			spells[2].cooldown = 10;
			spells[2].targettype = TARGET_ATTACKING;
			spells[2].instant = true;
			spells[2].perctrigger = RandomFloat(20.0f);
			spells[2].attackstoptimer = 1000;
			m_spellcheck[2] = true;

		}

		void OnCombatStart(Unit* mTarget)
		{
			CastTime();
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void CastTime()
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = spells[i].cooldown;
		}

		void OnTargetDied(Unit* mTarget)
		{
		}

		void OnCombatStop(Unit* mTarget)
		{
			CastTime();
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			CastTime();
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					spells[i].casttime--;

					if(m_spellcheck[i])
					{
						spells[i].casttime = spells[i].cooldown;
						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
						}

						if(spells[i].speech != "")
						{
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
							_unit->PlaySoundToSet(spells[i].soundid);
						}

						m_spellcheck[i] = false;
						return;
					}

					if((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						m_spellcheck[i] = true;
					}
					comulativeperc += spells[i].perctrigger;
				}
			}
		}

	protected:

		int nrspells;
};


class LordIncendius : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(LordIncendius);
		SP_AI_Spell spells[3];
		bool m_spellcheck[3];

		LordIncendius(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 3;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(13899);
			spells[0].cooldown = 10;
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].perctrigger = RandomFloat(20.0f);
			spells[0].attackstoptimer = 1000;
			m_spellcheck[0] = true;


			spells[1].info = dbcSpell.LookupEntry(13900);
			spells[1].cooldown = 10;
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = true;
			spells[1].perctrigger = RandomFloat(20.0f);
			spells[1].attackstoptimer = 1000;
			m_spellcheck[1] = true;


			spells[2].info = dbcSpell.LookupEntry(14099);
			spells[2].cooldown = 10;
			spells[2].targettype = TARGET_ATTACKING;
			spells[2].instant = true;
			spells[2].perctrigger = RandomFloat(20.0f);
			spells[2].attackstoptimer = 1000;
			m_spellcheck[2] = true;

		}

		void OnCombatStart(Unit* mTarget)
		{
			CastTime();
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void CastTime()
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = spells[i].cooldown;
		}

		void OnTargetDied(Unit* mTarget)
		{
		}

		void OnCombatStop(Unit* mTarget)
		{
			CastTime();
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			CastTime();
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					spells[i].casttime--;

					if(m_spellcheck[i])
					{
						spells[i].casttime = spells[i].cooldown;
						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
						}

						if(spells[i].speech != "")
						{
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
							_unit->PlaySoundToSet(spells[i].soundid);
						}

						m_spellcheck[i] = false;
						return;
					}

					if((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						m_spellcheck[i] = true;
					}
					comulativeperc += spells[i].perctrigger;
				}
			}
		}

	protected:

		int nrspells;
};


class LordRoccor : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(LordRoccor);
		SP_AI_Spell spells[3];
		bool m_spellcheck[3];

		LordRoccor(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 3;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(10448);
			spells[0].cooldown = 10;
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].perctrigger = RandomFloat(20.0f);
			spells[0].attackstoptimer = 1000;
			m_spellcheck[0] = true;


			spells[1].info = dbcSpell.LookupEntry(6524);
			spells[1].cooldown = 10;
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = true;
			spells[1].perctrigger = RandomFloat(20.0f);
			spells[1].attackstoptimer = 1000;
			m_spellcheck[1] = true;


			spells[2].info = dbcSpell.LookupEntry(10414);
			spells[2].cooldown = 10;
			spells[2].targettype = TARGET_ATTACKING;
			spells[2].instant = true;
			spells[2].perctrigger = RandomFloat(20.0f);
			spells[2].attackstoptimer = 1000;
			m_spellcheck[2] = true;

		}

		void OnCombatStart(Unit* mTarget)
		{
			CastTime();
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void CastTime()
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = spells[i].cooldown;
		}

		void OnTargetDied(Unit* mTarget)
		{
		}

		void OnCombatStop(Unit* mTarget)
		{
			CastTime();
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			CastTime();
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					spells[i].casttime--;

					if(m_spellcheck[i])
					{
						spells[i].casttime = spells[i].cooldown;
						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
						}

						if(spells[i].speech != "")
						{
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
							_unit->PlaySoundToSet(spells[i].soundid);
						}

						m_spellcheck[i] = false;
						return;
					}

					if((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						m_spellcheck[i] = true;
					}
					comulativeperc += spells[i].perctrigger;
				}
			}
		}

	protected:

		int nrspells;
};


class Magmus : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(Magmus);
		SP_AI_Spell spells[2];
		bool m_spellcheck[2];

		Magmus(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 2;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(13900);
			spells[0].cooldown = 10;
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].perctrigger = RandomFloat(20.0f);
			spells[0].attackstoptimer = 1000;
			m_spellcheck[0] = true;


			spells[1].info = dbcSpell.LookupEntry(24375);
			spells[1].cooldown = 10;
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = true;
			spells[1].perctrigger = RandomFloat(20.0f);
			spells[1].attackstoptimer = 1000;
			m_spellcheck[1] = true;
		}

		void OnCombatStart(Unit* mTarget)
		{
			CastTime();
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void CastTime()
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = spells[i].cooldown;
		}

		void OnTargetDied(Unit* mTarget)
		{
		}

		void OnCombatStop(Unit* mTarget)
		{
			CastTime();
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			CastTime();
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					spells[i].casttime--;

					if(m_spellcheck[i])
					{
						spells[i].casttime = spells[i].cooldown;
						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
						}

						if(spells[i].speech != "")
						{
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
							_unit->PlaySoundToSet(spells[i].soundid);
						}

						m_spellcheck[i] = false;
						return;
					}

					if((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						m_spellcheck[i] = true;
					}
					comulativeperc += spells[i].perctrigger;
				}
			}
		}

	protected:

		int nrspells;
};

class OkThorTheBreaker : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(OkThorTheBreaker);
		SP_AI_Spell spells[2];
		bool m_spellcheck[2];

		OkThorTheBreaker(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 2;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(15453);
			spells[0].cooldown = 10;
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].perctrigger = RandomFloat(20.0f);
			spells[0].attackstoptimer = 1000;
			m_spellcheck[0] = true;


			spells[1].info = dbcSpell.LookupEntry(15451);
			spells[1].cooldown = 10;
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = true;
			spells[1].perctrigger = RandomFloat(20.0f);
			spells[1].attackstoptimer = 1000;
			m_spellcheck[1] = true;

		}

		void OnCombatStart(Unit* mTarget)
		{
			CastTime();
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void CastTime()
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = spells[i].cooldown;
		}

		void OnTargetDied(Unit* mTarget)
		{
		}

		void OnCombatStop(Unit* mTarget)
		{
			CastTime();
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			CastTime();
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					spells[i].casttime--;

					if(m_spellcheck[i])
					{
						spells[i].casttime = spells[i].cooldown;
						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
						}

						if(spells[i].speech != "")
						{
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
							_unit->PlaySoundToSet(spells[i].soundid);
						}

						m_spellcheck[i] = false;
						return;
					}

					if((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						m_spellcheck[i] = true;
					}
					comulativeperc += spells[i].perctrigger;
				}
			}
		}

	protected:

		int nrspells;
};

class Phalanx : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(Phalanx);
		SP_AI_Spell spells[3];
		bool m_spellcheck[3];

		Phalanx(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 3;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(8732);
			spells[0].cooldown = 10;
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].perctrigger = RandomFloat(20.0f);
			spells[0].attackstoptimer = 1000;
			m_spellcheck[0] = true;


			spells[1].info = dbcSpell.LookupEntry(22425);
			spells[1].cooldown = 10;
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = true;
			spells[1].perctrigger = RandomFloat(20.0f);
			spells[1].attackstoptimer = 1000;
			m_spellcheck[1] = true;


			spells[2].info = dbcSpell.LookupEntry(14099);
			spells[2].cooldown = 10;
			spells[2].targettype = TARGET_ATTACKING;
			spells[2].instant = true;
			spells[2].perctrigger = RandomFloat(20.0f);
			spells[2].attackstoptimer = 1000;
			m_spellcheck[2] = true;

		}

		void OnCombatStart(Unit* mTarget)
		{
			CastTime();
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void CastTime()
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = spells[i].cooldown;
		}

		void OnTargetDied(Unit* mTarget)
		{
		}

		void OnCombatStop(Unit* mTarget)
		{
			CastTime();
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			CastTime();
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					spells[i].casttime--;

					if(m_spellcheck[i])
					{
						spells[i].casttime = spells[i].cooldown;
						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
						}

						if(spells[i].speech != "")
						{
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
							_unit->PlaySoundToSet(spells[i].soundid);
						}

						m_spellcheck[i] = false;
						return;
					}

					if((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						m_spellcheck[i] = true;
					}
					comulativeperc += spells[i].perctrigger;
				}
			}
		}

	protected:

		int nrspells;
};

class PrincessMoiraBronzebeard : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(PrincessMoiraBronzebeard);
		SP_AI_Spell spells[2];
		bool m_spellcheck[2];

		PrincessMoiraBronzebeard(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 2;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(10947);
			spells[0].cooldown = 10;
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].perctrigger = RandomFloat(20.0f);
			spells[0].attackstoptimer = 1000;
			m_spellcheck[0] = true;


			spells[1].info = dbcSpell.LookupEntry(22645);
			spells[1].cooldown = 10;
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = true;
			spells[1].perctrigger = RandomFloat(20.0f);
			spells[1].attackstoptimer = 1000;
			m_spellcheck[1] = true;

		}

		void OnCombatStart(Unit* mTarget)
		{
			CastTime();
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void CastTime()
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = spells[i].cooldown;
		}

		void OnTargetDied(Unit* mTarget)
		{
		}

		void OnCombatStop(Unit* mTarget)
		{
			CastTime();
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			CastTime();
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					spells[i].casttime--;

					if(m_spellcheck[i])
					{
						spells[i].casttime = spells[i].cooldown;
						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
						}

						if(spells[i].speech != "")
						{
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
							_unit->PlaySoundToSet(spells[i].soundid);
						}

						m_spellcheck[i] = false;
						return;
					}

					if((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						m_spellcheck[i] = true;
					}
					comulativeperc += spells[i].perctrigger;
				}
			}
		}

	protected:

		int nrspells;
};

class PyromancerLoregrain : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(PyromancerLoregrain);
		SP_AI_Spell spells[2];
		bool m_spellcheck[2];

		PyromancerLoregrain(Creature* pCreature) : CreatureAIScript(pCreature)
		{
			nrspells = 2;
			for(int i = 0; i < nrspells; i++)
			{
				m_spellcheck[i] = false;
			}

			spells[0].info = dbcSpell.LookupEntry(10448);
			spells[0].cooldown = 10;
			spells[0].targettype = TARGET_ATTACKING;
			spells[0].instant = true;
			spells[0].perctrigger = RandomFloat(20.0f);
			spells[0].attackstoptimer = 1000;
			m_spellcheck[0] = true;


			spells[1].info = dbcSpell.LookupEntry(15095);
			spells[1].cooldown = 10;
			spells[1].targettype = TARGET_ATTACKING;
			spells[1].instant = true;
			spells[1].perctrigger = RandomFloat(20.0f);
			spells[1].attackstoptimer = 1000;
			m_spellcheck[1] = true;

		}

		void OnCombatStart(Unit* mTarget)
		{
			CastTime();
			RegisterAIUpdateEvent(_unit->GetBaseAttackTime(MELEE));
		}

		void CastTime()
		{
			for(int i = 0; i < nrspells; i++)
				spells[i].casttime = spells[i].cooldown;
		}

		void OnTargetDied(Unit* mTarget)
		{
		}

		void OnCombatStop(Unit* mTarget)
		{
			CastTime();
			_unit->GetAIInterface()->setCurrentAgent(AGENT_NULL);
			_unit->GetAIInterface()->SetAIState(STATE_IDLE);
			RemoveAIUpdateEvent();
		}

		void OnDied(Unit* mKiller)
		{
			CastTime();
			RemoveAIUpdateEvent();
		}

		void AIUpdate()
		{
			float val = RandomFloat(100.0f);
			SpellCast(val);
		}

		void SpellCast(float val)
		{
			if(_unit->GetCurrentSpell() == NULL && _unit->GetAIInterface()->getNextTarget())
			{
				float comulativeperc = 0;
				Unit* target = NULL;
				for(int i = 0; i < nrspells; i++)
				{
					spells[i].casttime--;

					if(m_spellcheck[i])
					{
						spells[i].casttime = spells[i].cooldown;
						target = _unit->GetAIInterface()->getNextTarget();
						switch(spells[i].targettype)
						{
							case TARGET_SELF:
							case TARGET_VARIOUS:
								_unit->CastSpell(_unit, spells[i].info, spells[i].instant);
								break;
							case TARGET_ATTACKING:
								_unit->CastSpell(target, spells[i].info, spells[i].instant);
								break;
							case TARGET_DESTINATION:
								_unit->CastSpellAoF(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), spells[i].info, spells[i].instant);
								break;
						}

						if(spells[i].speech != "")
						{
							_unit->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
							_unit->PlaySoundToSet(spells[i].soundid);
						}

						m_spellcheck[i] = false;
						return;
					}

					if((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
					{
						_unit->setAttackTimer(spells[i].attackstoptimer, false);
						m_spellcheck[i] = true;
					}
					comulativeperc += spells[i].perctrigger;
				}
			}
		}

	protected:

		int nrspells;
};
void SetupBlackrockDepths(ScriptMgr* mgr)
{
	mgr->register_creature_script(9156, &AmbassadorFlamelash::Create);
	mgr->register_creature_script(9031, &AnubShiah::Create);
	mgr->register_creature_script(9016, &BaelGar::Create);
	mgr->register_creature_script(9019, &EmperorDagranThaurissan::Create);
	mgr->register_creature_script(9029, &Eviscerator::Create);
	mgr->register_creature_script(9056, &FineousDarkvire::Create);
	mgr->register_creature_script(9033, &GeneralAngerforge::Create);
	mgr->register_creature_script(8983, &GolemLordArgelmach::Create);
//mgr->register_creature_script(9027, &GoroshtheDervish::Create); - AI must be added ;)
	mgr->register_creature_script(9028, &Grizzle::Create);
	mgr->register_creature_script(9032, &HedrumTheCreeper::Create);
	mgr->register_creature_script(9018, &HighInterrogatorGerstahn::Create);
	mgr->register_creature_script(9319, &HoundmasterGrebmar::Create);
	mgr->register_creature_script(9537, &HurleyBlackbreath::Create);
	mgr->register_creature_script(9017, &LordIncendius::Create);
	mgr->register_creature_script(9025, &LordRoccor::Create);
	mgr->register_creature_script(9938, &Magmus::Create);
	mgr->register_creature_script(9030, &OkThorTheBreaker::Create);
	mgr->register_creature_script(9502, &Phalanx::Create);
	mgr->register_creature_script(8929, &PrincessMoiraBronzebeard::Create);
	mgr->register_creature_script(9024, &PyromancerLoregrain::Create);
}